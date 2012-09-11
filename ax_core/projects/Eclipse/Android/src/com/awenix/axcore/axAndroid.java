package com.awenix.axcore;

import android.app.Activity;
import android.os.Bundle;

public class axAndroid extends Activity {
    @Override 
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        jniOnInit( this );
    }
     
    public static native void jniOnInit	( Activity a ); 
}
