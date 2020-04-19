package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;

public class SVActivity extends AppCompatActivity {
    public SeekBar volumeSeekBar, vibrationSeekBar;
    public TextView volumeTextView, vibrationTextView;

    public int volume, vibration;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();
        setContentView(R.layout.activity_sv);

        volumeSeekBar = (SeekBar) findViewById(R.id.volumeSeekBar);
        vibrationSeekBar = (SeekBar) findViewById(R.id.vibrationSeekBar);
        volumeTextView = (TextView) findViewById(R.id.volumeTextView);
        vibrationTextView = (TextView) findViewById(R.id.vibrationTextView);
        
        volumeSeekBar.setProgress(Integer.parseInt(FirebaseFetchService.getVolume()));
        vibrationSeekBar.setProgress(Integer.parseInt(FirebaseFetchService.getVibration()));

        volumeSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                volume = seekBar.getProgress();
                FirebaseFetchService.setVolume(volume+"");
                try
                {
                    BluetoothCommService.sendData("volume: "+volume+"\n");
                }
                catch (IOException e)
                {

                }
            }
        });

        vibrationSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                vibration = seekBar.getProgress();
                FirebaseFetchService.setVibration(vibration+"");
                try
                {
                    BluetoothCommService.sendData("vibration: "+vibration+"\n");
                }
                catch (IOException e)
                {

                }
            }
        });
    }
}
