package com.example.rushi.smartwatch;

import android.app.Activity;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.ArrayList;

import static com.example.rushi.smartwatch.SOSActivity.deletedContactPosition;

public class ContactCustomAdapter extends ArrayAdapter<Contact> {

    private Activity context;
    private ArrayList<Contact> contacts;

    public ContactCustomAdapter(Activity context, ArrayList<Contact> contacts)
    {
        super(context, R.layout.list_contact, contacts);
        this.context = context;
        this.contacts = contacts;
    }

    @Override
    public View getView(int position, View view, ViewGroup parent)
    {
        LayoutInflater inflater = context.getLayoutInflater();
        View rowView = inflater.inflate(R.layout.list_contact, null, false);

        if(deletedContactPosition == position)
        {
            rowView.setBackgroundColor(Color.GRAY);
        }

        TextView contactNameTextView = (TextView) rowView.findViewById(R.id.contactNameTextView);
        TextView contactPhoneTextView = (TextView) rowView.findViewById(R.id.contactPhoneTextView);
        TextView contactEmailTextView = (TextView) rowView.findViewById(R.id.contactEmailTextView);


        contactNameTextView.setText(contacts.get(position).getName());
        contactPhoneTextView.setText(contacts.get(position).getPhone());
        contactEmailTextView.setText(contacts.get(position).getEmail());

        return rowView;
    }
}
