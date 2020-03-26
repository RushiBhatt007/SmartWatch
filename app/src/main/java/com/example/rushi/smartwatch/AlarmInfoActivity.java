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
                int hour = alarmTimePicker.getHour();
                int minute = alarmTimePicker.getMinute();
                String message = messageEditText.getText().toString();

                if(!message.equals(""))
                {
                    newAlarm = new Alarm(hour+":"+minute, message);
                    database.child("alarm").setValue(newAlarm);
                    Intent nextActivity = new Intent(AlarmInfoActivity.this, AlarmActivity.class);
                    startActivity(nextActivity);
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
