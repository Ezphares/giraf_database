package dk.aau.cs.giraf.puddle;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.Statement;

import android.database.Cursor;
import android.os.AsyncTask;
import android.util.Log;

public class UploadTask extends AsyncTask{

    private static final String TAG = "UploadTask";
    Database db = null;

    @Override
    protected Object doInBackground(Object... params) {
        Log.i(TAG, "Hello from " + TAG);

        db = MainActivity.db;

        String url = "jdbc:mysql://10.0.0.10:3306/test";
        String user = "admin";
        String password = "1234";
        PreparedStatement pst = null;

        try {
            Log.i(TAG, "Trying connection");
            Class.forName("com.mysql.jdbc.Driver").newInstance();
            Connection con = DriverManager.getConnection(url, user, password);
            Statement st = con.createStatement();

            Log.i(TAG, "Uploading changes");
            Cursor getnew = db.getNew();
            while(getnew.moveToNext()) {
                String id = getnew.getString(0);
                String test = getnew.getString(1);

                pst = con.prepareStatement("INSERT INTO `user` (id, username, password, certificate) VALUES(?, ?, ?, ?)");
                pst.setString(1, id);
                pst.setString(2, test);
                pst.setString(3, test);
                pst.setString(4, test);
                pst.addBatch();
                Log.i("getNew", "getting " + id + test + pst);
            }
            db.close();
            con.setAutoCommit(false);
            pst.executeBatch();
            con.setAutoCommit(true);
            Log.i(TAG, "Test");

            pst.close();
            st.close();
            con.close();
        } catch (Exception e) {
        }

        // TODO Auto-generated method stub
        return null;
    }

}
