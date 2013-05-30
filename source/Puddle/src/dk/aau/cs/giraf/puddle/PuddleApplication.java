package dk.aau.cs.giraf.puddle;

import android.app.Application;
import android.util.Log;

/** Application makes sure that the database is accessible the entire time the application is running. */
public class PuddleApplication extends Application{
	
	private static final String TAG = "PuddleApplication";
	private Database db = null;

	@Override
	public void onCreate() {
	    super.onCreate();

	    Log.d(TAG, "onCreate started");

	    setDatabase(new Database(this));

	}

	public Database getDatabase() {
	    return db;
	}

	public void setDatabase(Database db) {
	    this.db = db;
	}
}