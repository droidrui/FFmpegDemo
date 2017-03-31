package com.droidrui.ffmpegdemo.jni;

/**
 * Created by Administrator on 2017/3/29.
 */

public class VideoJNI {

    public native void initDecode(Object surface, String inputPath, String outputPath);

    public native void startDecode();

    public native void stopDecode();

    public native void initEncode(String outputPath);

    public native void startEncode();

    public native void stopEncode();

}
