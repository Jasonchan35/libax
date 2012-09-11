package com.awenix.axcore;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Display;
import android.view.MotionEvent;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class axGLApp extends axAndroid {
    private MyView	view_;
    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
              
//        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN,
  //              			    WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
//        Log.v( "libax","src = " + getApplicationInfo().sourceDir );
  //      Log.v( "libax","res = " + getPackageResourcePath() );
        
    	Display display = getWindowManager().getDefaultDisplay(); 
        view_ = new MyView(getApplication(), display.getWidth(), display.getHeight() );        
        setContentView(view_);
        //setContentView(R.layout.main);
        jniOnCreate();
    }
    @Override protected void onStart() {
        super.onStart();
    }
    @Override protected void onRestart() {
    	super.onRestart();
    }
    @Override protected void onResume() {
        super.onResume();
        jniOnBecomeActive();
    }
    @Override protected void onPause() {
    	jniOnResignActive();
        super.onPause();
    }
    @Override protected void onStop() {
    	jniOnEnterBackground();
        jniOnDestroy();
        super.onStop();
    }
    @Override protected void onDestroy() {
        super.onDestroy();
    }
    
    public void finish() {
    	super.finish();
    }
   	
    public static native void jniOnDidCreateOpenGL	();
	public static native void jniOnCreate			();
	public static native void jniOnEnterForeground	();
	public static native void jniOnBecomeActive		();
	public static native void jniOnResignActive		();
	public static native void jniOnEnterBackground	();
	public static native void jniOnDestroy			();
	 
	public static native void jniOnFrame			();
	public static native void jniOnResize			( int width,  int height );
	public static native void jniOnTouchEvent		( int action, int touchId, float x, float y, float pressure, long timestamp );
	
	private static class MyRenderer implements GLSurfaceView.Renderer {
		@Override public void onDrawFrame(GL10 gl) {
//			gl.glClearColor( 0,1,0,1 );
//			gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
			jniOnFrame();
	    }
	    
	    @Override public void onSurfaceChanged(GL10 gl, int width, int height) {
	    	jniOnResize( width, height ); 
	    }
	
	    @Override public void onSurfaceCreated(GL10 gl, EGLConfig config) {	    	
	    	jniOnDidCreateOpenGL();
	    }
	}	
			 
	private class MyView extends GLSurfaceView {		
		public MyView(Context context, int width, int height) {
		    super(context);
		    setRenderer(new MyRenderer());
		    getHolder().setKeepScreenOn(true);
		}
		
		@Override 
		public boolean onTouchEvent( MotionEvent ev ) {	   	
			int 	count = ev.getPointerCount();
			int 	action;
			int 	idx;
			
			switch( ev.getAction() ) {
				case MotionEvent.ACTION_CANCEL:
				case MotionEvent.ACTION_MOVE: {
					action = 3;
					for( idx=0; idx<count; idx++ ) {
						axGLApp.jniOnTouchEvent(action, 
								ev.getPointerId(idx), 
								ev.getX(idx), 
								ev.getY(idx), 
								ev.getPressure(idx),
								ev.getEventTime() );
					}
				} return true;
			}
				
			switch( ev.getActionMasked() ) {
		    	case MotionEvent.ACTION_DOWN:
		    	case MotionEvent.ACTION_POINTER_DOWN: 
		    		action = 1; idx = ev.getActionIndex(); break;
		    	
		    	case MotionEvent.ACTION_UP:
		    	case MotionEvent.ACTION_POINTER_UP:	
		    		action = 2; idx = ev.getActionIndex(); break;	
		    	
		    	default: return false;
			}
							
			axGLApp.jniOnTouchEvent(action, 
									ev.getPointerId(idx), 
									ev.getX(idx), 
									ev.getY(idx), 
									ev.getPressure(idx),
									ev.getEventTime() );
			return true;
		}   
	}

}
