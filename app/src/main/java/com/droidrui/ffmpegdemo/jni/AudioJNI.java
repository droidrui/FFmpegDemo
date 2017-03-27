package com.droidrui.ffmpegdemo.jni;

/**
 * Created by lingrui on 2017/3/24.
 */

public class AudioJNI {

    public native void initPlay(int framesPerBuffer, String filePath);

    public native void startPlay();

    public native void stopPlay();

}
