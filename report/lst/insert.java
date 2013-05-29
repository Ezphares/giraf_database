public void insertUser(String id, String username, String password, String certificate, String timestamp){
    Log.i(TAG, "Inserting user " + id + " " + username + " " + password + " " + certificate + " " + timestamp);

    SQLiteDatabase db = dbHelper.getWritableDatabase();

    String sql = "INSERT OR REPLACE INTO user (id, username, password, certificate, timestamp) " +
            "VALUES ('" + id + "', '" + username + "', '" + password + "', '" + certificate + "', '" + timestamp + "')";

    db.execSQL(sql);
}