package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class AlarmActivity extends AppCompatActivity {

    public static ArrayList<Alarm> alarmList = new ArrayList<Alarm>();
    public static AlarmCustomAdapter alarmArrayAdapter;

    private ListView alarmListView;
    private Button alarmAddButton, alarmDeleteButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();
        setContentView(R.layout.activity_alarm);

        alarmListView = (ListView) findViewById(R.id.alarmListView);
        alarmAddButton = (Button) findViewById(R.id.alarmAddButton);
        alarmDeleteButton = (Button) findViewById(R.id.alarmDeleteButton);

        alarmList = FirebaseFetchService.getAlarms();
        alarmArrayAdapter = new AlarmCustomAdapter(this, alarmList);

        alarmListView.setAdapter(alarmArrayAdapter);

        alarmAddButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent nextActivity = new Intent(AlarmActivity.this, AlarmInfoActivity.class);
                startActivity(nextActivity);
            }
        });
    }

    public static ArrayAdapter<Alarm> getAlarmArrayAdapter()
    {
        return alarmArrayAdapter;
    }
}