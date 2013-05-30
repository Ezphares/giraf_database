public Cursor getUpdated(String table) {
    SQLiteDatabase db = dbHelper.getReadableDatabase();

    String getLastSync = "SELECT * FROM last_sync;";

    Cursor cursor = db.rawQuery(getLastSync, null);
    String last_sync = "";
    if(cursor != null) {
        cursor.moveToFirst();
        last_sync = cursor.getString(1);
    }

    Log.i("Get Updated", "Last sync = " + last_sync);

    String sql = "SELECT * FROM " + table + " WHERE timestamp > " + last_sync + ";";

    return db.rawQuery(sql, null);
}