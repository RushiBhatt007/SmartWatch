package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class AlarmActivity extends AppCompatActivity {

    ArrayList<Alarm> alarmArrayList = new ArrayList<Alarm>();
    ArrayAdapter<Alarm> alarmArrayAdapter;

    private Button alarmAddButton, alarmDeleteButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();
        setContentView(R.layout.activity_alarm);

        alarmAddButton = (Button) findViewById(R.id.alarmAddButton);
        alarmDeleteButton = (Button) findViewById(R.id.alarmDeleteButton);

        alarmArrayAdapter = new ArrayAdapter<Alarm>(this, android.R.layout.simple_list_item_1, alarmArrayList);

        alarmAddButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent nextActivity = new Intent(AlarmActivity.this, AlarmInfoActivity.class);
                startActivity(nextActivity);
            }
        });
    }
}
