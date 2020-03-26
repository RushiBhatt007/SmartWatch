package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class SVActivity extends AppCompatActivity {
    public SeekBar volumeSeekBar, vibrationSeekBar;
    public TextView volumeTextView, vibrationTextView;

    public int volume, vibration;
    public DatabaseReference database;
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

        database = FirebaseDatabase.getInstance().getReference();

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
                database.child("volume").setValue(volume);
                FirebaseFetchService.setVolume(volume+"");
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
                database.child("vibration").setValue(vibration);
                FirebaseFetchService.setVibration(vibration+"");
            }
        });
    }
}
