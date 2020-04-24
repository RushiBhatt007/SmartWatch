package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TimePicker;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class AlarmInfoActivity extends AppCompatActivity {

    private TimePicker alarmTimePicker;
    private EditText messageEditText;
    private Button alarmSaveButton;

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
        alarmSaveButton = (Button) findViewById(R.id.alarmSaveButton);

        alarmSaveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int hr = alarmTimePicker.getHour();
                int min = alarmTimePicker.getMinute();
                int ampm;   // 0 is AM, 1 is PM
                if(hr > 12)
                {
                    ampm = 1;
                    hr = hr-12;
                }
                else if(hr == 0)
                {
                    hr = hr+12;
                    ampm = 0;
                }
                else if(hr == 12)
                {
                    ampm = 1;
                }
                else
                {
                    ampm = 0;
                }
                String hour = hr+"";
                String minute = min>9?min+"":"0"+min;

                String alarmTime = hour+":"+minute+" ";
                alarmTime = alarmTime + (ampm==0?"AM":"PM");

                String message = messageEditText.getText().toString();

                if(!message.equals(""))
                {
                    newAlarm = new Alarm(alarmTime, hour, minute, ampm, message);
                    Long longTime = System.currentTimeMillis()/1000;
                    String timestamp = longTime.toString();
                    FirebaseFetchService.addAlarms(newAlarm, timestamp);

                    Handler handler1 = new Handler();
                    handler1.postDelayed(new Runnable() {
                        public void run()
                        {
                            BluetoothCommService.updateAlarmTime();
                        }
                    }, 1000);

                    Handler handler2 = new Handler();
                    handler2.postDelayed(new Runnable() {
                        public void run()
                        {
                            BluetoothCommService.updateAlarmMessage();
                        }
                    }, 1000);

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
