package com.example.myapplication;


import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;


public class RendererJNI implements GLSurfaceView.Renderer {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public RendererJNI(Context context) {

    }

    public native void glesInit();
    public native void glesRender();
    public native void glesResize(int width, int height);

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // TODO Auto-generated method stub
        glesInit();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        // TODO Auto-generated method stub
        glesResize(width, height);

    }
    public void onDrawFrame(GL10 gl) {
        // TODO Auto-generated method stub
        glesRender();
    }
}
