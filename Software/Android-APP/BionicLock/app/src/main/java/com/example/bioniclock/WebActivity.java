package com.example.bioniclock;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

public class WebActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_web);

        Intent intent = getIntent();
        String msg = "http://" + intent.getStringExtra("ip");
        //Toast.makeText(WebActivity.this, msg, Toast.LENGTH_LONG).show();

        WebView webView = (WebView)findViewById(R.id.web_view);
        //webView.loadUrl("http://192.168.31.4");
        webView.loadUrl(msg);
        webView.setWebViewClient(new WebViewClient(){
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }
        });
    }
}


