package dk.aau.cs.giraf.puddle;

import java.io.File;
import android.net.Uri;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {

    static Database db = null;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /** Setting a custom layout for the list activity */
        setContentView(R.layout.activity_main);

        db = ((PuddleApplication) getApplication()).getDatabase();

        /** Referencing buttons in activity_main.xml */
        Button buttonOpen = (Button) findViewById(R.id.buttonOpen);
        Button buttonSync = (Button) findViewById(R.id.buttonSync);
        Button buttonInsert = (Button) findViewById(R.id.buttonInsert);
        Button buttonDownload = (Button) findViewById(R.id.buttonDownload);

        /** Defining a click event listener for the button "Open Database" */
        OnClickListener openListener = new OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i("Open button", "Hello from open button");

                Intent intent = new Intent();
                File file = new File("/sdcard/PuddleSync/giraflocal.sqlite");
                intent.setDataAndType(Uri.fromFile(file), "sqlite/*");
                startActivity(intent);
            }
        };

        /** Defining a click event listener for the button "Synchronize" */
        OnClickListener synchronizeListener = new OnClickListener() {
            @SuppressWarnings("unchecked")
            @Override
            public void onClick(View v) {
                Log.i("Sync button", "Hello from sync button");

                new SyncTask().execute();

            }
        };

        /** Defining a click event listener for the button "Insert" */
        OnClickListener insertListener = new OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i("Insert button", "Hello from insert button");

                String currentTime = String.valueOf(System.currentTimeMillis());

                //db.insert("12", "hello world from android" + currentTime, currentTime);
            }
        };

        /** Defining a click event listener for the button "Download" */
        OnClickListener downloadListener = new OnClickListener() {
            @SuppressWarnings("unchecked")
            @Override
            public void onClick(View v) {
                Log.i("Insert button", "Hello from insert button");

                new DownloadTask().execute();

            }
        };

        /** Setting event listeners for buttons */
        buttonOpen.setOnClickListener(openListener);
        buttonSync.setOnClickListener(synchronizeListener);
        buttonInsert.setOnClickListener(insertListener);
        buttonDownload.setOnClickListener(downloadListener);
    }
}
