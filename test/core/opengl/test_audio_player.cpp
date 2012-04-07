#include <ax/ax_App.h>
#include <ax/ax_core.h>



class Mode0 : public axGLAppMode {
public:

	axAudioPlayer ap;
	
	
	virtual axStatus	onCreate() {

		
		axPRINT_FUNC_NAME;
		axStatus	st;
		
		st = ap.init( "test_audio.mp3" ); if( !st ) return st;
		ap.play();
		
		
		return 0;
	}

	virtual	void	onFrame( axGLAppRenderRequest &req ) {
		req.ui.showLayout = true;
		
		glClearColor( axColorRGBf( 1, 1, 1 ) );
		glClearBuffers();
		gl_2D_mode();
		glEnable( GL_BLEND );
		

		
	}
/*
	virtual void onKeyEvent( axGLAppKeyEvent &ev ) {
		ax_log( "key event {?}", ev );
	}
	
	virtual void onTouchEvent( axGLAppTouchEvent &ev ) {
		ax_log( "touch event {?}", ev );
		if( ev.touches.size() == 3 ) {
//			quitMode();
		}
	}

	virtual void onMouseEvent( axGLAppMouseEvent &ev ) {
		if( ev.isAnyMove() ) return;
//		ax_log( "mouse event {?}", ev );
		
		if( ev.isDown(1) ) {
		//	app_->setContentSize( 300 + 20, 200 + 20 );
			app_->setFrameRate( 60 );
		}else if( ev.isDown(2) ) {
		//	app_->setContentSize( 640, 400 );
			app_->setFrameRate( 1 );
		}
	}
 */
};

axGLApp_program_main( "axTest_GLApp", Mode0 );


