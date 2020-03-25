package com.example.rushi.smartwatch;

import com.google.firebase.database.IgnoreExtraProperties;

@IgnoreExtraProperties
public class Alarm {
    public String alarmTime;
    public String message;

    public Alarm(){}

    public Alarm(String alarmTime, String message)
    {
        this.alarmTime = alarmTime;
        this.message = message;
    }

    public String getAlarmTime()
    {
        return alarmTime;
    }

    public String getMessage()
    {
        return message;
    }
}
