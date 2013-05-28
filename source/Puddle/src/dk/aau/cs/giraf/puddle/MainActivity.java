package dk.aau.cs.giraf.puddle;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

import java.sql.PreparedStatement;

public class MainActivity extends Activity {

    static Database db = null;

    /** URL, username and password for MySQL database. */
    static String url = "jdbc:mysql://10.0.0.10:3306/giraf";
    static String user = "admin";
    static String password = "1234";

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /** Setting a custom layout for the list activity */
        setContentView(R.layout.activity_main);

        // References the application, making database accessible AsyncTasks.
        db = ((PuddleApplication) getApplication()).getDatabase();

        /** Referencing buttons in activity_main.xml */
        Button buttonSync = (Button) findViewById(R.id.buttonSync);
        Button buttonInsert = (Button) findViewById(R.id.buttonInsert);

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

                db.insertUser("2", "Replacing Entry", "APassword", "ACertificate", currentTime);
            }
        };

        /** Setting event listeners for buttons */
        buttonSync.setOnClickListener(synchronizeListener);
        buttonInsert.setOnClickListener(insertListener);
    }
}
