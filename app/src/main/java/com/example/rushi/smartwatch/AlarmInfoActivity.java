package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TimePicker;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class AlarmInfoActivity extends AppCompatActivity {

    private TimePicker alarmTimePicker;
    private EditText messageEditText;
    private Button saveButton;

    private String remainingTime;
    Alarm newAlarm;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Intent intent = new Intent();
        setContentView(R.layout.activity_alarm_info);

        alarmTimePicker = (TimePicker) findViewById(R.id.alarmTimePicker);
        messageEditText = (EditText) findViewById(R.id.messageEditText);
        saveButton = (Button) findViewById(R.id.saveButton);

        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int hr = alarmTimePicker.getHour();
                int min = alarmTimePicker.getMinute();
                String ampm;
                if(hr > 12)
                {
                    ampm = " PM";
                    hr = hr-12;
                }
                else if(hr == 0)
                {
                    hr = hr+12;
                    ampm = " AM";
                }
                else if(hr == 12)
                {
                    ampm = " PM";
                }
                else
                {
                    ampm = " AM";
                }
                String hour = hr+"";
                String minute = min>9?min+"":"0"+min;

                String message = messageEditText.getText().toString();

                if(!message.equals(""))
                {
                    newAlarm = new Alarm(hour+":"+minute+ampm, message);
                    Long longTime = System.currentTimeMillis()/1000;
                    String timestamp = longTime.toString();
                    FirebaseFetchService.addAlarms(newAlarm, timestamp);
                    Intent nextActivity = new Intent(AlarmInfoActivity.this, AlarmActivity.class);
                    startActivity(nextActivity);
                    remainingTime = hour+":"+minute+ampm;
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
