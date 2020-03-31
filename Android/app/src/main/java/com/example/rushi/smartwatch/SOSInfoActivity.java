package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.util.Patterns;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class SOSInfoActivity extends AppCompatActivity {

    private EditText contactNameEditText, contactPhoneEditText, contactEmailEditText;
    private Button contactSaveButton;

    Contact newContact;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = new Intent();
        setContentView(R.layout.activity_sos_info);

        contactNameEditText = (EditText) findViewById(R.id.contactNameEditText);
        contactPhoneEditText = (EditText) findViewById(R.id.contactPhoneEditText);
        contactEmailEditText = (EditText) findViewById(R.id.contactEmailEditText);
        contactSaveButton = (Button) findViewById(R.id.contactSaveButton);

        contactSaveButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view)
            {
                String name = contactNameEditText.getText().toString();
                String phone = contactPhoneEditText.getText().toString();
                String email = contactEmailEditText.getText().toString();
                if(!isValidEmail(email))
                {
                    msg("Enter a valid Email ID");
                }
                else if(!isValidPhone(phone))
                {
                    msg("Enter a valid Phone Number");
                }
                else if(name.equals("") || phone.equals("") || email.equals(""))
                {
                    msg("Please fill all the fields");
                }
                else
                {
                    newContact = new Contact(name, phone, email);
                    Long longTime = System.currentTimeMillis()/1000;
                    String timestamp = longTime.toString();
                    FirebaseFetchService.addContact(newContact, timestamp);
                    Intent nextActivity = new Intent(SOSInfoActivity.this, SOSActivity.class);
                    startActivity(nextActivity);
                }
            }
        });
    }

    private boolean isValidEmail(String s)
    {
        return Patterns.EMAIL_ADDRESS.matcher(s).matches();
    }

    private boolean isValidPhone(String s)
    {
        return Patterns.PHONE.matcher(s).matches();
    }

    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
    }
}
