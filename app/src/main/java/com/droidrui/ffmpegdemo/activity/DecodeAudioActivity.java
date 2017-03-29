package com.droidrui.ffmpegdemo.activity;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

import com.droidrui.ffmpegdemo.R;
import com.droidrui.ffmpegdemo.jni.AudioJNI;

import java.io.File;

public class DecodeAudioActivity extends AppCompatActivity {

    private Button mBtn;
    private int mState = 0;
    private AudioJNI mAudioJNI;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_decode_audio);

        initView();
        mAudioJNI = new AudioJNI();
        File input = new File(Environment.getExternalStorageDirectory(), "00.mp3");
        File output = new File(Environment.getExternalStorageDirectory(), "record.pcm");
        mAudioJNI.initDecode(1024, input.getAbsolutePath(), output.getAbsolutePath());
    }

    private void initView() {
        mBtn = (Button) findViewById(R.id.btn);
        mBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setState();
            }
        });
    }

    private void setState() {
        switch (mState) {
            case 0:
                mState = 1;
                mBtn.setText("停止");
                mAudioJNI.startDecode();
                break;
            case 1:
                mState = 0;
                mBtn.setText("播放");
                mAudioJNI.stopDecode();
                break;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mState == 1) {
            mState = 0;
            mAudioJNI.stopDecode();
        }
    }
}
