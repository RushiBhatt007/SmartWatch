package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class AlarmActivity extends AppCompatActivity {

    public static ArrayList<Alarm> alarmList = new ArrayList<Alarm>();
    public AlarmCustomAdapter alarmArrayAdapter;
    public static int deletedAlarmPosition = -1;

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
                deletedAlarmPosition = -1;
                alarmArrayAdapter.notifyDataSetChanged();
                Intent nextActivity = new Intent(AlarmActivity.this, AlarmInfoActivity.class);
                startActivity(nextActivity);
            }
        });

        alarmListView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> adapterView, View view, int position, long id) {
                deletedAlarmPosition = position;
                alarmArrayAdapter.notifyDataSetChanged();
                return false;
            }
        });

        alarmDeleteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(deletedAlarmPosition != -1)
                {
                    FirebaseFetchService.deleteAlarm(alarmList.get(deletedAlarmPosition).getAlarmTime(), alarmList.get(deletedAlarmPosition).getMessage());
                    alarmArrayAdapter.remove(alarmList.get(deletedAlarmPosition));
                    deletedAlarmPosition = -1;
                    alarmArrayAdapter.notifyDataSetChanged();
                }
            }
        });
    }
}