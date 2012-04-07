#include <ax/ax_App.h>
#include <ax/ax_core.h>

class TestErr: public axNonCopyable {
public:
	
	double		d, e;
	uint8_t	t8;
	uint8_t	t16;
	uint64_t	t64;
	
	
	template< class S >
	axStatus serialize_io( S &s ) {
		axStatus st;
		
		st = s.io( t8 ); if( !st ) return st;
		st = s.io( d ); if( !st ) return st;
		st = s.io( t16 ); if( !st ) return st;
		st = s.io( e ); if( !st ) return st;
		st = s.io( t64 ); if( !st ) return st;
		
		return 0;
	}
	
};

axStatus test2() {
	axStatus st;
	TestErr a;
	a.d = 1231238345.678939836;
	a.e = 45678.12312311;
	a.t8 =1;
	a.t16 =2;

	axByteArray buf;
	st = ax_serialize_to_buf( buf, a ); if( !st ) return st;
	
	axDeserializer	de( buf );
	TestErr b;
	de.io( b );
	
	return 0;
};

axStatus test3() {
	axStringU	tmp;
	tmp.set( "ABCD12312" );
	ax_log( "testing = {?}", tmp );
	
#if axOS_iOS || axOS_MacOSX
	NSString *s = ax_toNSString( tmp );
	NSLog( @" NSString = %@", s );
#endif

	return 0;
};

class Mode0 : public axGLAppMode {
public:
	axGLUIButtonRef	btn[9];
	axGLTexture	tex;

	
	virtual	void	onFrame( axGLAppRenderRequest &req ) {
		req.ui.showLayout = true;
		
		glClearColor( axColorRGBf( 1, 1, 1 ) );
		glClearBuffers();
		ax_gl2DMode();
		glEnable( GL_BLEND );
		
		//tex.blt(0,0);
		
	}
	
	axStatus	test_9_box() {
		axStatus	st;
		
		//==
		for( int i=0; i<9; i++ ) {
			
			axGLUIButtonRef &b = btn[i];
			st = rootView->createChild( b );		if( !st ) return st;
			b->setFizedSize	( 10 * (i+1), 10 * (i+1) );
			b->setAlign	( 0,0 );
			b->setMargin ( 10 );
		}
		
		int i = 0;
		
		btn[i]->setAlign( -1, -1 ); i++;
		btn[i]->setAlign(  0, -1 ); i++;
		btn[i]->setAlign(  1, -1 ); i++;
		
		
		btn[i]->setAlign( -1, 0 ); i++;	
		btn[i]->setAlign(  0, 0 ); i++;
		btn[i]->setAlign(  1, 0 ); i++;
		
		btn[i]->setAlign( -1, 1 ); i++;
		btn[i]->setAlign(  0, 1 ); i++;
		btn[i]->setAlign(  1, 1 ); i++;
		
		
		/*
		 st = rootView->createChild( btn2 );		if( !st ) return st;
		 btn2->setFizedSize	( 20, 20 );
		 btn2->setAlign	( -1,0 );
		 btn2->setPadding ( 10 );
		 
		 st = rootView->createChild( btn3 );		if( !st ) return st;
		 btn3->setFizedSize	( 20, 20 );
		 btn3->setAlign	( 1,0 );
		 btn3->setPadding ( 10 );		
		 
		 */
		
		
		rootView->doLayout();	
		ax_log( " rootView = {?}", rootView->rect() );
		
		for( int i=0; i<9; i++ ) {	
			ax_log( " btn{?} = {?}", i, btn[i]->rect() );
		}
		
		return 0;
		
	}
	
	
	axStatus	test_v_box2() {
		/*
		float a=1234.5678;
		float f;
		f = ax_modf( a, &f );
		ax_log( "f {?} ", f );
		
		int c = 20;
		for( int i=0; i<c; i++ ) {
			
			int p = c/2 + ( i%2?(i+1)/2*-1:(i+1)/2 );
			ax_log( "p = {?}" , p);
			
		}*/
		
		
		
		axStatus	st;
		axGLUIVBoxRef	vbox;
		st = rootView->createChild( vbox );		if( !st ) return st;
		vbox->setPadding( 10 );
		vbox->setMargin( 5 );	
		vbox->setSpacing( 0 );

		for( int i=0; i<9; i++ ) {
			
			axGLUIButtonRef &b = btn[i];
			st = vbox->createChild( b );		if( !st ) return st;
			//b->setFizedSize	( 10 * (i+1), 10 * (i+1) );
			//b->setProportion( 0.1f * (i+1), 0.1f * (i+1) );
			b->setProportion( 0.10123f * (i+1), 0.1f * (i+1) );
			b->setAlign	( 0,0 );
			b->setMargin ( 1 );
		}
		
		
		btn[3]->setMinRequireW( 250 );		
		btn[5]->setMaxRequireW( 10 );		
		
		btn[1]->setMinRequireH( 50 );		
		btn[7]->setMaxRequireH( 10 );
		
		rootView->doLayout();	
		ax_log( " rootView = {?}", rootView->rect() );
		
		for( int i=0; i<9; i++ ) {	
			ax_log( " btn{?} = {?}", i, btn[i]->rect() );
		}
		
		return 0;
		
	}
	
	
	axStatus	test_v_box3() {

		axStatus	st;
		axGLUIVBoxRef	vbox;
		st = rootView->createChild( vbox );		if( !st ) return st;
		vbox->setPadding( 10 );
		vbox->setMargin( 5 );	
		vbox->setSpacing( 0 );
		vbox->setAlign	( -1,-1 );
		
		int c = 3;
		
		for( int i=0; i<c; i++ ) {
			
			axGLUIButtonRef &b = btn[i];
			st = vbox->createChild( b );		if( !st ) return st;

			b->setProportion( 1,1 );
			b->setAlign	( 0,0 );
			b->setMargin ( 1 );
		}
		
		
		btn[0]->setMaxRequireH( 200 );
		btn[1]->setMaxRequireH( 50 );		
		btn[2]->setMaxRequireH( 50 );		

		
		rootView->doLayout();	
		ax_log( " rootView = {?}", rootView->rect() );
		
		for( int i=0; i<c; i++ ) {	
			ax_log( " btn{?} = {?}", i, btn[i]->rect() );
		}
		
		return 0;
		
	}
	
	
	
	axStatus	test_v_box() {
		axStatus	st;
		axGLUIVBoxRef	vbox;
		st = rootView->createChild( vbox );		if( !st ) return st;
		vbox->setPadding( 10 );
		vbox->setMargin( 5 );	
		vbox->setSpacing( 0 );


		st = vbox->createChild( btn[0] );		if( !st ) return st;
		//		btn1->setPos	( 20, 20 );
		//		btn1->setSize	( 100, 30 );
		//		btn1->setProportion( 0,0 );
		btn[0]->setAlign	( 1,0 );
		btn[0]->setMargin	( 10 );
		btn[0]->setProportion( 0.5, 0.5 );

		st = vbox->createChild( btn[1] );		if( !st ) return st;
		//		btn2->setPos	( 20, 120 );
		//		btn2->setSize	( 100, 30 );
		btn[1]->setAlign	( 0,0 );
		btn[1]->setProportion( 0.5, 0.5 );

		st = vbox->createChild( btn[2] );		if( !st ) return st;
		//		btn3->setPos	( 20, 120 );
		//		btn3->setSize	( 100, 30 );
		btn[2]->setAlign	( -1,0 );
		btn[2]->setProportion( 0.5, 0.5 );

		rootView->doLayout();	

		ax_log( " rootView = {?}", rootView->rect() );

		ax_log( " vbox = {?}", vbox->rect() );
		ax_log( " vbox contentSize = {?}", vbox->contentSize() );


		ax_log( " btn0 = {?}", btn[0]->rect() );
		ax_log( " btn1 = {?}", btn[1]->rect() );
		ax_log( " btn2 = {?}", btn[2]->rect() );
	
		return 0;
	
	}
	
	axStatus	test_scale_view() {
		axStatus	st;
		axGLUIScaleViewRef	scaleView;
		st = rootView->createChild( scaleView );	if( !st ) return st;
		scaleView->setVirtualContentSize( 320, 460 );

		
		st = scaleView->createChild( btn[0] );		if( !st ) return st;

		btn[0]->setFizedSize( 160, 160 );
		btn[0]->setAlign	( 0, 0 );
		btn[0]->setMargin ( 0 );
			
 

		rootView->doLayout();	

		ax_log( " rootView = {?}", rootView->rect() );
		ax_log( " scaleView = {?} ContentSize {?} scale {?}", scaleView->rect(), scaleView->contentSize(), scaleView->scale() );
		ax_log( " btn0 = {?}", btn[0]->rect() );
		
		return 0;

	}
	
	axStatus	test_scale_view2() {
		axStatus	st;
		axGLUIScaleViewRef	scaleView;
		st = rootView->createChild( scaleView );	if( !st ) return st;
		scaleView->setProportion( 1, 1 );
		scaleView->setVirtualContentSize( 300, 300 );
		scaleView->setKeepAspectRatio( true );
		scaleView->setAlign	( -1, 0 );
		scaleView->setMaxRequireW( 10 );
		
		st = scaleView->createChild( btn[0] );		if( !st ) return st;

		btn[0]->setFizedSize( 160, 160 );
		btn[0]->setAlign	( 0, 0 );
		btn[0]->setMargin ( 0 );
			
		//scaleView->setScale( 1, 1 );
		//scaleView->setScale( 0.5,0.5 );

		rootView->doLayout();	

		ax_log( " rootView = {?}", rootView->rect() );
		ax_log( " scaleView = {?} ContentSize {?} scale {?}", scaleView->rect(), scaleView->contentSize(), scaleView->scale() );
		ax_log( " btn0 = {?}", btn[0]->rect() );
		
		return 0;
	}
	
	
	axStatus	test_label() {
		axStatus	st;
		axGLUITextRef text;
		axGLUITextRef text2;
		
		st = rootView->createChild( text );	if( !st ) return st;
		text->setAlign( -1, -1 );
		text->setProportion( 0.5, 0.7);
		text->setText( L"ABABABABABABABABABABABABABABABABABABABABABABABABABAB" );
		//text->setFontSize( 26 );


		//rootView->doLayout();	
	
		return 0;
	}
	
	virtual axStatus	onCreate() {


		axTimeStamp t1, t2;
		axDateTime	dt;
		t1.now();
		dt.set( t1 );
		
		ax_log( "Timestamp = {?}, {?}", t1, dt );
		
		axPRINT_FUNC_NAME;
		axStatus	st;

		//st = test_scale_view();
		st = test_label();
		
		t2.now();
		
		
		ax_log( "t1 = {?}  t2 = {?} diff - {?}", t1.second(), t2.second(), t2.second()-t1.second() );

		

		axStringA str_a; str_a.set( "hello world 1" );
		//axStringW str_w; str_w.set( L"hello world 2" );	
				
		axByteArray buf1, buf2;
				
		//const axExternalByteArray &ex_array = str_a.as_ByteArray();
		
		axExternalByteArray	ex( (uint8_t*)str_a.c_str(), str_a.size() );
		ax_encode_xor( buf1, ex, 0x5c );
		
		/*
		ax_encode_xor( ex_array, 0x5c );
		ax_encode_xor( ex_array, 0x5c );
		
		axStringA t;
		t.set( (const char*)ex_array.ptr() );
		
		ax_log( "t {?}" , t );
		*/
		/*				
	
		
		const axExternalByteArray ex_array = str_a.as_ByteArray();
		ax_encode_xor( buf1, ex_array, 0x5c );					   
		
		ax_log( "buf1 {?}" , buf1 );
		
		ax_encode_xor( buf2, buf1, 0x5c );	
		
		ax_log( "buf2 {?}" , buf2 );
		
		axStringA t;
		t.set( (const char*)buf2.ptr() );
		
		ax_log( "t {?}" , t );
		*/
		return 0;
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


