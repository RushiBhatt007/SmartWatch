package com.example.rushi.smartwatch;

import com.google.firebase.database.IgnoreExtraProperties;

@IgnoreExtraProperties
public class Contact {
    public String name;
    public String phone;
    public String email;

    public Contact(){}

    public Contact(String name, String phone, String email)
    {
        this.name = name;
        this.phone = phone;
        this.email = email;
    }

    public String getName()
    {
        return name;
    }

    public String getPhone()
    {
        return phone;
    }

    public String getEmail()
    {
        return email;
    }
}
