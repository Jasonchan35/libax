#include <ax/ax_core.h>


class Mode0 : public axGLAppMode {
public:
	
	axGLTexture	tex0, tex1, tex_png;
	axImage png_img;
	
	float	animationTime;
	
	virtual axStatus	onCreate() {	
		axPRINT_FUNC_NAME;	
		axStatus	st;
//		axGLArrayBuffer	vbuf;		
//		glVertexPointer( vbuf );
		
		axArray<int>	a;
		for( axSize i=0; i<10; i++ ) {
			a.append( i*10 );
		}
		
		ax_log( "{?}", a );		
		st = a.removeBySwap( 9, 2 );
		ax_log( "{?}\n   {?}", st , a );
		
		app().enableAccelerometer( true );
		
		
//		st = axFileSystem::saveFile( buf, "test_tmp.txt" );	if( !st ) return st;
		
//		ax_log( "numberOfProcessors = {?}", axSystem::numberOfProcessors() );
		
		animationTime = 0;
		app().setTitleName( L"Test App 1" );
				

		int tw = 300;
		int th = 200;

		axImage	img;
		st = img.create( axColor::t_RGBAb, tw, th );			if( !st ) return st;
		st = img.setAll( axColorRGBAb( 255, 0,0, 128 ) );		if( !st ) return st;
		
		for( int y=0; y<img.height(); y++ ) {
			axColorRGBAb* c;
			img.pixelPointer( c, 0, y );
			for( int x=0; x<img.width(); x++ ) {
				c[x].set( 50, 50, y, 255 );
			}
		}
		st = tex0.create( img );		if( !st ) return st;


		axFont		font;

		font.create( NULL, 24 );
		img.createFromTextInRect( font, L"LINE1\n123456789\nABCDE", tw, th, axFont::kAlignCenterXY );
		tex1.create( img );
		

		st = png_img.loadFile( "test_img.png" );
		//if( !st ) return st;
		st = tex_png.create( png_img );
		//if( !st ) return st;
		
		ax_log( "onCreate OK");
		return 0;
	}

	virtual	void	onFrame( axGLAppRenderRequest &req ) {
		glClearColor( axColorRGBf( 1, 1, 1 ) );
		glClearBuffers();
		gl_2D_mode();
		glEnable( GL_BLEND );
		
		glColor3f(1,1,1);		tex0.blt( 10, 10 );
		glColor3f(1,1,0);		tex1.blt( 10, 10 );
	
		glColor3f(1,1,1);
		tex_png.blt( 0,100 );
		
		glDrawGradient( axRect2f( 100, 200, 200, 200 ), 
						axColorRGBf(1,0,0), axColorRGBf(1,1,0), 
						axColorRGBf(0,0,1), axColorRGBf(0,1,0) );

		{ axScope_glPushMatrix	pm;
			glTranslate( 160, 100, 0 );
			glRotate( animationTime, 0,0,1 );			
			glColor4f(1,0,0, 0.5);
			glDraw( axRect2f( 0, 0, 50, 50 ) );
		}
			
		animationTime += req.frameTime() * 180;
	}

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
};

axGLApp_program_main( "axTest_GLApp", Mode0 );


