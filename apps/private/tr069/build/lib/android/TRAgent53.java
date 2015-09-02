package com.example.TRAgent53;

import android.app.Activity;
import android.os.Bundle;

public class TRAgent53 extends Activity {
    /** Called when the activity is first created. */
	public native void OneAgentMainLoop();
	/*
	 * this is used to load the 'oneagent-jni' library on application startup.
	 * The library has already been unpacked into
	 * /data/data/com.example.TRAgent53/lib/libtragent-jni.so at installation time by the
	 * package manager.
	 */
	static {
		System.loadLibrary("tragent-jni");
	}
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        JNI jni = new JNI();
        jni.OneAgentMainLoop();
    }
}
