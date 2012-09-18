package com.awenix.axcore;

import android.graphics.*;
//import android.util.*;

public class axFont {
	private	Typeface typeface_;
	private float	 size_;
	
	public int  create( String face, float size, int style ) {
		if( face.length() == 0 ) {
			typeface_ = Typeface.defaultFromStyle( style );
		}else{
			typeface_ = Typeface.create( face, style );			
		}
		size_ = size;
		return 0;
	}
	
	private Paint newPaint() { 
		Paint p = new Paint();
		p.setTextSize	(size_);
		p.setTypeface	(typeface_);
		p.setAntiAlias	(true);
		return p;
	}	
	
	public Bitmap makeImageInRect( String text, int width, int height, int align ) {
		Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		bitmap.eraseColor(0);
		Paint paint = newPaint();
		paint.setColor(0xffffffff);
		
		float x = 0; //default to Left
		if( (align & 0x01) != 0  ) {
			paint.setTextAlign( Paint.Align.LEFT );
			x = 0;
		}else if( (align & 0x02) != 0 ) {
			paint.setTextAlign( Paint.Align.RIGHT );
			x = width;
		}else if( (align & 0x04) != 0 ){
			paint.setTextAlign( Paint.Align.CENTER );			
			x = width / 2;
		}

		String[] s = text.split("\\\n");
		
		float lineSpace = paint.getFontSpacing();
		
		float y = lineSpace; //default to top
		if( (align & 0x10 ) != 0 ) {
			y = lineSpace;
		}else if( (align & 0x20 ) != 0 ) {
			y = height - 1;
		}else if( (align & 0x40 ) != 0 ) {
			float th = (s.length) * lineSpace;
			y = (height - th) / 2 + lineSpace - 1;	
		}
		
		y -= paint.descent();
		
		for( int i=0; i<s.length; i++ ) {
			canvas.drawText( s[i], x, y, paint );
			y += paint.getFontSpacing();
		}
		return bitmap;
	}
		
	public Rect getTextRect( String text ) {
		Rect rect = new Rect();
		String[] s = text.split("\\\n");
		
		Paint paint = newPaint();
		//Paint.FontMetrics m = paint.getFontMetrics();
			
		int w = 0;
		int h = 0;
		
		for( int i=0; i<s.length; i++ ) {
			paint.getTextBounds( s[i], 0, s[i].length(), rect );
			int w0 = rect.right - rect.left;
			if( w0 > w ) w = w0; 
			h += paint.getFontSpacing();
		}
		
		h +=  paint.descent();
		
		rect.set(0,0,w,h);
		return rect;
	}
}
