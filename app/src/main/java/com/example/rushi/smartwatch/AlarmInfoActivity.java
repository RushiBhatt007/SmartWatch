package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TimePicker;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class AlarmInfoActivity extends AppCompatActivity {

    private TimePicker alarmTimePicker;
    private EditText messageEditText;
    private Button saveButton;

    private String remainingTime;
    Alarm newAlarm;
    public DatabaseReference database;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Intent intent = new Intent();
        setContentView(R.layout.activity_alarm_info);

        alarmTimePicker = (TimePicker) findViewById(R.id.alarmTimePicker);
        messageEditText = (EditText) findViewById(R.id.messageEditText);
        saveButton = (Button) findViewById(R.id.saveButton);

        database = FirebaseDatabase.getInstance().getReference();

        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int hr = alarmTimePicker.getHour();
                int min = alarmTimePicker.getMinute();
                String hour = hr+"";
                String minute = min>9?min+"":"0"+min;

                String message = messageEditText.getText().toString();

                if(!message.equals(""))
                {
                    newAlarm = new Alarm(hour+":"+minute, message);
                    Long longTime = System.currentTimeMillis()/1000;
                    String timestamp = longTime.toString();
                    database.child("alarm").child(timestamp).setValue(newAlarm);
                    FirebaseFetchService.addAlarms(newAlarm);
                    Intent nextActivity = new Intent(AlarmInfoActivity.this, AlarmActivity.class);
                    startActivity(nextActivity);
                    remainingTime = hour+":"+minute;
                    msg("Alarm set for "+remainingTime+" from now");
                }
                else
                {
                    msg("Please enter a message");
                }
            }
        });
    }

    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
    }
}
