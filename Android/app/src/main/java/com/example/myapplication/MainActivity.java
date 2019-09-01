package com.example.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.util.Log;


public class MainActivity extends AppCompatActivity {
    private final int CONTEXT_CLIENT_VERSION = 3;
    private GLSurfaceView mGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mGLSurfaceView = new GLSurfaceView(this);
        if (detectOpenGLES20())
        {
            mGLSurfaceView.setEGLContextClientVersion(CONTEXT_CLIENT_VERSION);
            mGLSurfaceView.setRenderer(new RendererJNI(this));
        }
        else{
            Log.e("opengles20", "OpenGL ES 2.0 not supported on device.  Exiting...");
            finish();
        }

        setContentView(mGLSurfaceView);
    }


    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
        mGLSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        // TODO Auto-generated method stub
        super.onPause();
        mGLSurfaceView.onPause();

    }

    private boolean detectOpenGLES20() {
        ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return true;
        //return (info.reqGlEsVersion >= 0x30000);

    }
}
