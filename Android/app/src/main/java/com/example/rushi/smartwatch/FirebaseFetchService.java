package com.example.rushi.smartwatch;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
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

    public static ArrayList<Contact> contacts;
    public static int numberOfContacts;

    public static String GMailUsername, GMailPassword;
    public static String longitude, latitude;

    public static String selectedMode, hourLong, hourShort, minuteLong, minuteShort;

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
            contacts = new ArrayList<Contact>();
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
                    longitude = dataSnapshot.child("longitude").getValue().toString();
                    latitude = dataSnapshot.child("latitude").getValue().toString();

                    GMailUsername = dataSnapshot.child("GMailUsername").getValue().toString();
                    GMailPassword = dataSnapshot.child("GMailPassword").getValue().toString();

                    volume = dataSnapshot.child("volume").getValue().toString();
                    vibration = dataSnapshot.child("vibration").getValue().toString();

                    selectedMode = dataSnapshot.child("mode").child("selectedMode").getValue().toString();
                    hourLong = dataSnapshot.child("mode").child("hourLong").getValue().toString();
                    hourShort = dataSnapshot.child("mode").child("hourShort").getValue().toString();
                    minuteLong = dataSnapshot.child("mode").child("minuteLong").getValue().toString();
                    minuteShort = dataSnapshot.child("mode").child("minuteShort").getValue().toString();
                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });
            STATUS=1;

            Handler handler1 = new Handler();
            handler1.postDelayed(new Runnable() {
                public void run()
                {
                    //BluetoothCommService.updateAlarmMessage();
                }
            }, 4000);

            Handler handler2 = new Handler();
            handler2.postDelayed(new Runnable() {
                public void run()
                {
                    //BluetoothCommService.updateSV();
                    //BluetoothCommService.updateAlarmTime();
                }
            }, 6000);

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

    public static String getLongitude()
    {
        return longitude;
    }

    public static String getLatitude()
    {
        return latitude;
    }

    public static String getVolume()
    {
        return volume;
    }

    public static String getVibration()
    {
        return vibration;
    }

    public static String getSelectedMode()
    {
        return selectedMode;
    }

    public static String getHourLong()
    {
        return hourLong;
    }

    public static String getHourShort()
    {
        return hourShort;
    }

    public static String getMinuteLong()
    {
        return minuteLong;
    }

    public static String getMinuteShort()
    {
        return minuteShort;
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

    public static String getGMailUsername()
    {
        return GMailUsername;
    }

    public static String getGMailPassword()
    {
        return GMailPassword;
    }

    public static void setLongitude(String longitude1)
    {
        database.child("longitude").setValue(longitude1);
    }

    public static void setLatitude(String latitude1)
    {
        database.child("latitude").setValue(latitude1);
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

    public static void setSelectedMode(String selectedMode1)
    {
        selectedMode = selectedMode1;
        database.child("mode").child("selectedMode").setValue(selectedMode);
    }

    public static void setHourLong(String hourLong1)
    {
        hourLong = hourLong1;
        database.child("mode").child("hourLong").setValue(hourLong);
    }

    public static void setHourShort(String hourShort1)
    {
        hourShort = hourShort1;
        database.child("mode").child("hourShort").setValue(hourShort);
    }

    public static void setMinuteLong(String minuteLong1)
    {
        minuteLong = minuteLong1;
        database.child("mode").child("minuteLong").setValue(minuteLong);
    }

    public static void setMinuteShort(String minuteShort1)
    {
        minuteShort = minuteShort1;
        database.child("mode").child("minuteShort").setValue(minuteShort);
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