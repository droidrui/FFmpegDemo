package com.droidrui.ffmpegdemo.jni;

/**
 * Created by lingrui on 2017/3/24.
 */

public class AudioJNI {

    public native void initDecode(int bufferSize, String inputPath, String outputPath);

    public native void startDecode();

    public native void stopDecode();

    public native void initEncode(int bufferSize, String inputPath, String outputPath);

    public native void startEncode();

    public native void stopEncode();

}
