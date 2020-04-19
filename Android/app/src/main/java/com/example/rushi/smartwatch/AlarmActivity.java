package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;
import java.util.ArrayList;

public class AlarmActivity extends AppCompatActivity {

    public static ArrayList<Alarm> alarmList = new ArrayList<Alarm>();
    public AlarmCustomAdapter alarmCustomAdapter;
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
        alarmCustomAdapter = new AlarmCustomAdapter(this, alarmList);

        alarmListView.setAdapter(alarmCustomAdapter);

        alarmAddButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(FirebaseFetchService.getNumberOfAlarms()<=3)
                {
                    deletedAlarmPosition = -1;
                    alarmCustomAdapter.notifyDataSetChanged();
                    Intent nextActivity = new Intent(AlarmActivity.this, AlarmInfoActivity.class);
                    startActivity(nextActivity);
                }
                else
                {
                    Toast.makeText(getApplicationContext(), "Can not add more than 4 alarms", Toast.LENGTH_SHORT).show();
                }

                try
                {
                    BluetoothCommService.sendData("add alarm"+"\n");
                }
                catch (IOException e)
                {

                }
            }
        });

        alarmListView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> adapterView, View view, int position, long id) {
                deletedAlarmPosition = position;
                alarmCustomAdapter.notifyDataSetChanged();
                return false;
            }
        });

        alarmDeleteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(deletedAlarmPosition != -1)
                {
                    FirebaseFetchService.deleteAlarm(alarmList.get(deletedAlarmPosition).getAlarmTime(), alarmList.get(deletedAlarmPosition).getMessage());
                    alarmCustomAdapter.remove(alarmList.get(deletedAlarmPosition));
                    deletedAlarmPosition = -1;
                    alarmCustomAdapter.notifyDataSetChanged();

                    try
                    {
                        BluetoothCommService.sendData("delete alarm"+"\n");
                    }
                    catch (IOException e)
                    {

                    }
                }
            }
        });
    }
}