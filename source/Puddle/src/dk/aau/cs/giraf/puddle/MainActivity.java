package dk.aau.cs.giraf.puddle;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {

    static Database db = null;

    /** URL, username and password for MySQL database. */
    static String DB_URL = "jdbc:mysql://10.0.0.12:3306/giraf";
    static String DB_USER = "root";
    static String DB_PASS = "1234";

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

                String timestamp = String.valueOf(System.currentTimeMillis());
                
                db.insertUser("3", "Tom K", "newpass", "certified yes 1", timestamp);
                db.insertUser("4", "John N", "password", "cerified yes 2", timestamp);
                db.insertProfile("4", "Tom K", "78376", null, "anEmail", "1", "AnAddress", null, "4", "1", "1", timestamp);
                db.insertDepartment("5", "New Department", "Street 3", "636", "test@emailprovider", "1", "1", timestamp);
                db.insertTag("4", "Test Tags", timestamp);
            }
        };

        /** Setting event listeners for buttons */
        buttonSync.setOnClickListener(synchronizeListener);
        buttonInsert.setOnClickListener(insertListener);
    }
}
