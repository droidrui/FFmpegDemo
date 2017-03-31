package com.droidrui.ffmpegdemo.activity;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.droidrui.ffmpegdemo.R;

public class MainActivity extends AppCompatActivity {


    static {
        System.loadLibrary("media");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }

    public void decodeAudio(View view) {
        startActivity(new Intent(this, DecodeAudioActivity.class));
    }

    public void decodeVideo(View view) {
        startActivity(new Intent(this, DecodeVideoActivity.class));
    }

    public void encodeAudio(View view) {
        startActivity(new Intent(this, EncodeAudioActivity.class));
    }

    public void encodeVideo(View view) {
        startActivity(new Intent(this, EncodeVideoActivity.class));
    }
}
