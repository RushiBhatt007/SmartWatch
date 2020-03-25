package com.example.rushi.smartwatch;

import com.google.firebase.database.IgnoreExtraProperties;

@IgnoreExtraProperties
public class Contact {
    public String name;
    public String phoneNumber;
    public String emailID;

    public Contact(){}

    public Contact(String name, String phoneNumber, String emailID)
    {
        this.name = name;
        this.phoneNumber = phoneNumber;
        this.emailID = emailID;
    }

    public String getName()
    {
        return name;
    }

    public String getPhoneNumber()
    {
        return phoneNumber;
    }

    public String getEmailID()
    {
        return emailID;
    }
}
