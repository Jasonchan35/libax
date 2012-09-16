package com.awenix.axcore;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Display;
import android.view.MotionEvent;
import android.util.Log;
import android.widget.RelativeLayout;
import android.widget.EditText;
import android.view.ViewGroup;
import android.view.ViewGroup.MarginLayoutParams;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView.OnEditorActionListener;
import android.widget.TextView;
import android.view.inputmethod.EditorInfo;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;


import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class axGLApp extends axAndroid {

    private MyView	view_;
    private EditText et_;
	private RelativeLayout rl_;
    
    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
              
//        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN,
  //              			    WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
//        Log.v( "libax","src = " + getApplicationInfo().sourceDir );
  //      Log.v( "libax","res = " + getPackageResourcePath() );
        
    	Display display = getWindowManager().getDefaultDisplay(); 
        view_ = new MyView(getApplication(), display.getWidth(), display.getHeight() );
        
		et_ = new EditText(  this );
        et_.setVisibility( View.GONE );
		et_.setImeOptions( EditorInfo.IME_ACTION_DONE );
		
		et_.setOnEditorActionListener( new OnEditorActionListener() {
            @Override
            public boolean onEditorAction( TextView v, int actionId, KeyEvent event) {
            
                if (actionId == EditorInfo.IME_ACTION_DONE ) {                	
                	Log.v("libax-java","Test: " + et_.getText() );
                	jniOnKeyboardInput( et_.getText().toString() );
                	hideKeyboard();
                    return true;
                }

                return false;
            }
        });
        

                
        rl_ = new RelativeLayout(this);
        
        RelativeLayout.LayoutParams lp = editText_Params( 0 );

        rl_.addView( view_ );
        rl_.addView( et_, lp );
        
        setContentView( rl_ );
        
     
        getWindow().setSoftInputMode( WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE ) ;
        
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
	public static native void jniOnKeyboardInput	( String s );
	
	
	RelativeLayout.LayoutParams editText_Params ( int h ) {	
	
		ViewGroup.MarginLayoutParams mlp = new ViewGroup.MarginLayoutParams( LayoutParams.FILL_PARENT, h );
 
		mlp.leftMargin = 10;
		mlp.rightMargin = 10;
		
		RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams( mlp );
	
        lp.addRule(RelativeLayout.ALIGN_PARENT_LEFT );
        lp.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM );
	
		return lp;
	}
	
	public void hideKeyboard() {
		if( et_.getVisibility() == View.INVISIBLE ) return;

	 	InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
       	imm.hideSoftInputFromWindow( et_.getWindowToken(), 0 );

		et_.setVisibility( View.GONE );
	}
	
	public void showKeyboard( int height, int max_line ) {
		
		rl_.removeView( et_ );
		
		et_.setText("");
		et_.setVisibility( View.VISIBLE );
		et_.setMaxLines( max_line );

		RelativeLayout.LayoutParams lp = editText_Params( height );
                
        rl_.addView( et_, lp );

    	et_.setFocusableInTouchMode(true);
    	et_.requestFocus();
    	
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);   	
    	imm.showSoftInput( et_, InputMethodManager.SHOW_FORCED);       	

	}
	
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
