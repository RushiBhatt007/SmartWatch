package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.widget.SeekBar;

import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class SVActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();
        setContentView(R.layout.activity_sv);

        SeekBar volumeSeekBar = (SeekBar) findViewById(R.id.volumeSeekBar);
        SeekBar vibrationSeekBar = (SeekBar) findViewById(R.id.vibrationSeekBar);

        DatabaseReference database = FirebaseDatabase.getInstance().getReference();

        database.setValue("1");
    }
}
