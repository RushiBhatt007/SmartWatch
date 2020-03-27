package com.example.rushi.smartwatch;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;

public class FirebaseFetchService extends Service {

    public static String volume=null, vibration=null, alarmIndex=null, contactIndex=null;

    public static ArrayList<Alarm> alarms = new ArrayList<Alarm>();

    public static int STATUS = 1;
    public DatabaseReference database;

    @Override
    public void onCreate()
    {
        super.onCreate();
        STATUS=0;
        database = FirebaseDatabase.getInstance().getReference();
    }

    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        if ("fetch".equals(intent.getAction()))
        {
            database.child("alarm").addListenerForSingleValueEvent(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    for (DataSnapshot snapshot : dataSnapshot.getChildren())
                    {
                        Alarm alarm = snapshot.getValue(Alarm.class);
                        alarms.add(alarm);
                        Log.e("FBA", "Error");
                    }
                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });


            database.addListenerForSingleValueEvent(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    volume = dataSnapshot.child("volume").getValue().toString();
                    vibration = dataSnapshot.child("vibration").getValue().toString();
                    alarmIndex = dataSnapshot.child("alarmIndex").getValue().toString();
                    contactIndex = dataSnapshot.child("contactIndex").getValue().toString();
                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });
            STATUS=1;
            return START_STICKY;
        }
        else
        {
            stopForeground(true);
            return START_NOT_STICKY;
        }
    }

    public static String getVolume()
    {
        return volume;
    }

    public static String getVibration()
    {
        return vibration;
    }

    public static String getAlarmIndex()
    {
        return alarmIndex;
    }

    public static String getContactIndex()
    {
        return contactIndex;
    }

    public static ArrayList<Alarm> getAlarms()
    {
        return alarms;
    }

    public static void setVolume(String volume1)
    {
        volume = volume1;
    }

    public static void setVibration(String vibration1)
    {
        vibration = vibration1;
    }

    public static void setAlarmIndex(String alarmIndex1)
    {
        alarmIndex = alarmIndex1;
    }

    public static void setContactIndex(String contactIndex1)
    {
        contactIndex = contactIndex1;
    }

    public static void addAlarms(Alarm alarm)
    {
        alarms.add(alarm);
    }

    public static int getSTATUS()
    {
        return STATUS;
    }

}