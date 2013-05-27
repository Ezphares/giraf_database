public Cursor getUpdatedUser() {
    SQLiteDatabase db = dbHelper.getReadableDatabase();

    String getLastSync = "SELECT * FROM last_sync;";

    Cursor cursor = db.rawQuery(getLastSync, null);
    String last_sync = "";
    if(cursor != null) {
        cursor.moveToFirst();
        last_sync = cursor.getString(1);
    }

    Log.i("Get Updated User", "Last sync = " + last_sync);

    String sql = "SELECT * FROM user WHERE modified > " + last_sync + ";";

    return db.rawQuery(sql, null);
}