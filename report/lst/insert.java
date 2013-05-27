public void insertUser(String id, String username, String password, String certificate, String lastSync){
    Log.i(TAG, "Inserting user " + id + " " + username + " " + password + " " + certificate + " " + lastSync);

    SQLiteDatabase db = dbHelper.getWritableDatabase();

    String sql = "INSERT OR REPLACE INTO user (id, username, password, certificate, modified) " +
            "VALUES ('" + id + "', '" + username + "', '" + password + "', '" + certificate + "', '" + lastSync + "')";

    db.execSQL(sql);
}