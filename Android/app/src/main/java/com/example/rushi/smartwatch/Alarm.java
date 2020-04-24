package com.example.rushi.smartwatch;

import com.google.firebase.database.IgnoreExtraProperties;

@IgnoreExtraProperties
public class Alarm {
    public String alarmTime, alarmHr, alarmMin, alarmMsg;
    public int alarmAMPM;

    public Alarm(){}

    public Alarm(String alarmTime, String alarmHr, String alarmMin, int alarmAMPM, String alarmMsg)
    {
        this.alarmTime = alarmTime;
        this.alarmHr = alarmHr;
        this.alarmMin = alarmMin;
        this.alarmAMPM = alarmAMPM;
        this.alarmMsg = alarmMsg;
    }

    public String getAlarmTime()
    {
        return alarmTime;
    }

    public String getAlarmHr()
    {
        return alarmHr;
    }

    public String getAlarmMin()
    {
        return alarmMin;
    }

    public int getAlarmAMPM()
    {
        return alarmAMPM;
    }

    public String getMessage()
    {
        return alarmMsg;
    }
}
