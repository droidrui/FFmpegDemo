package com.droidrui.ffmpegdemo.activity;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.droidrui.ffmpegdemo.R;

public class MainActivity extends AppCompatActivity {


    static {
        System.loadLibrary("audio");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }

    public void play(View view) {
        startActivity(new Intent(this, PlayActivity.class));
    }
}
