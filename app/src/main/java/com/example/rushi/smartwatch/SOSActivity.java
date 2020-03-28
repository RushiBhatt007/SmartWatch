package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class SOSActivity extends AppCompatActivity {

    public static ArrayList<Contact> contactList = new ArrayList<Contact>();
    public ContactCustomAdapter contactCustomAdapter;
    public static int deletedContactPosition = -1;

    private ListView contactListView;
    private Button contactAddButton, contactDeleteButton;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();
        setContentView(R.layout.activity_sos);

        contactListView = (ListView) findViewById(R.id.contactListView);
        contactAddButton = (Button) findViewById(R.id.contactAddButton);
        contactDeleteButton = (Button) findViewById(R.id.contactDeleteButton);

        contactList = FirebaseFetchService.getContacts();
        contactCustomAdapter = new ContactCustomAdapter(this, contactList);

        contactListView.setAdapter(contactCustomAdapter);

        contactAddButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(FirebaseFetchService.getNumberOfContacts()<=4)
                {
                    deletedContactPosition = -1;
                    contactCustomAdapter.notifyDataSetChanged();
                    Intent nextActivity = new Intent(SOSActivity.this, SOSInfoActivity.class);
                    startActivity(nextActivity);
                }
                else
                {
                    Toast.makeText(getApplicationContext(), "Can not add more than 5 contacts", Toast.LENGTH_SHORT).show();
                }
            }
        });

        contactListView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> adapterView, View view, int position, long id) {
                deletedContactPosition = position;
                contactCustomAdapter.notifyDataSetChanged();
                return false;
            }
        });

        contactDeleteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(deletedContactPosition != -1)
                {
                    FirebaseFetchService.deleteContact(contactList.get(deletedContactPosition).getName(), contactList.get(deletedContactPosition).getPhone(), contactList.get(deletedContactPosition).getEmail());
                    contactCustomAdapter.remove(contactList.get(deletedContactPosition));
                    deletedContactPosition = -1;
                    contactCustomAdapter.notifyDataSetChanged();
                }
            }
        });
    }
}
