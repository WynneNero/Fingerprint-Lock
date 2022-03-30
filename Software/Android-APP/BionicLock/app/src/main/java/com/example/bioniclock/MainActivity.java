package com.example.bioniclock;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    EditText editText;
    EditText edit_passwd;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editText = (EditText)findViewById(R.id.edit_ip);
        edit_passwd = (EditText)findViewById(R.id.edit_passwd);
        Button button = (Button)findViewById(R.id.btn_link);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!edit_passwd.getText().toString().equals("12345678")){
                    Toast.makeText(MainActivity.this, "密码错误", Toast.LENGTH_LONG).show();
                    return;
                }else{
                    Intent intent = new Intent(MainActivity.this, WebActivity.class);
                    intent.putExtra("ip", editText.getText().toString());
                    startActivity(intent);
                }

            }
        });
        SharedPreferences pref = getSharedPreferences("data", MODE_PRIVATE);
        String Ip = pref.getString("IP", "");
        String passwd = pref.getString("passwd", "");
        Boolean reme = pref.getBoolean("remember", false);
        editText.setText(Ip);
        edit_passwd.setText(passwd);
        CheckBox checkBox = (CheckBox)findViewById(R.id.checkb_remember);
        checkBox.setChecked(reme);
    }
    protected void onDestroy(){
        super.onDestroy();
        SharedPreferences.Editor editor = getSharedPreferences("data", MODE_PRIVATE).edit();
        CheckBox checkBox = (CheckBox)findViewById(R.id.checkb_remember);
        if(checkBox.isChecked()){
            String Ip = editText.getText().toString();
            String passwd = edit_passwd.getText().toString();
            editor.putString("IP", Ip);
            editor.putString("passwd", passwd);
            editor.putBoolean("remember", true);
            editor.apply();
        }else{
            editor.clear();
            editor.apply();
        }
    }
}

