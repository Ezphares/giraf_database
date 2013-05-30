package dk.aau.cs.giraf.puddle;

import android.os.AsyncTask;
import android.util.Log;

public class SyncTask extends AsyncTask{

    @SuppressWarnings("unchecked")
    @Override
    protected Object doInBackground(Object... params) {
        Log.i("SyncTask", "Hello from SyncTask");

        new UploadTask().execute();
        new DownloadTask().execute();
        return null;
    }

}
