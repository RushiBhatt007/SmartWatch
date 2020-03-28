package com.example.rushi.smartwatch;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

import androidx.annotation.NonNull;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;

public class FirebaseFetchService extends Service {

    public static String volume=null, vibration=null;

    public static ArrayList<Alarm> alarms;
    public static int numberOfAlarms;

    public static ArrayList<Contact> contacts = new ArrayList<Contact>();
    public static int numberOfContacts;

    public static int STATUS = -1;  //
    public static DatabaseReference database;

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
            numberOfAlarms = 0;
            alarms = new ArrayList<Alarm>();
            database.child("alarm").addListenerForSingleValueEvent(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    for (DataSnapshot snapshot : dataSnapshot.getChildren()) {
                        Alarm alarm = snapshot.getValue(Alarm.class);
                        alarms.add(alarm);
                        numberOfAlarms++;
                    }
                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });

            numberOfContacts = 0;
            //contacts = new ArrayList<Contact>();
            database.child("contact").addListenerForSingleValueEvent(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    for (DataSnapshot snapshot : dataSnapshot.getChildren()) {
                        Contact contact = snapshot.getValue(Contact.class);
                        contacts.add(contact);
                        numberOfContacts++;
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

    public static void deleteAlarm(final String deleteAlarmTime, final String deleteAlarmMessage)
    {
        database.child("alarm").addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for (DataSnapshot snapshot : dataSnapshot.getChildren())
                {
                    Alarm alarm = snapshot.getValue(Alarm.class);
                    if(alarm.getAlarmTime().equals(deleteAlarmTime) && alarm.getMessage().equals(deleteAlarmMessage))
                    {
                        snapshot.getRef().removeValue();
                        numberOfAlarms--;
                    }
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
    }

    public static void deleteContact(final String deleteContactName, final String deleteContactPhone, final String deleteContactEmail)
    {
        database.child("contact").addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for (DataSnapshot snapshot : dataSnapshot.getChildren())
                {
                    Contact contact = snapshot.getValue(Contact.class);
                    if(contact.getName().equals(deleteContactName) && contact.getPhone().equals(deleteContactPhone) && contact.getEmail().equals(deleteContactEmail))
                    {
                        snapshot.getRef().removeValue();
                        numberOfContacts--;
                    }
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
    }

    public static String getVolume()
    {
        return volume;
    }

    public static String getVibration()
    {
        return vibration;
    }

    public static ArrayList<Alarm> getAlarms()
    {
        return alarms;
    }

    public static int getNumberOfAlarms()
    {
        return numberOfAlarms;
    }

    public static ArrayList<Contact> getContacts()
    {
        return contacts;
    }

    public static int getNumberOfContacts()
    {
        return numberOfContacts;
    }

    public static void setVolume(String volume1)
    {
        volume = volume1;
        database.child("volume").setValue(volume);
    }

    public static void setVibration(String vibration1)
    {
        vibration = vibration1;
        database.child("vibration").setValue(vibration);
    }

    public static void addAlarms(Alarm alarm, String timestamp)
    {
        database.child("alarm").child(timestamp).setValue(alarm);
        alarms.add(alarm);
        numberOfAlarms++;
    }

    public static void addContact(Contact contact, String timestamp)
    {
        database.child("contact").child(timestamp).setValue(contact);
        contacts.add(contact);
        numberOfContacts++;
    }

    public static int getSTATUS()
    {
        return STATUS;
    }

}