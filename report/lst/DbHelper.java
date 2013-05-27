private class DbHelper extends SQLiteOpenHelper {
    public static final String DB_NAME = "giraflocal.sqlite";
    public static final int DB_VERSION = 1;
    private static final String TAG = "DbHelper";

    public DbHelper() {
        super(context, DB_NAME, null, DB_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        Log.i("Database onCreate", "Creating Database");
        String createUser = String.format("CREATE TABLE IF NOT EXISTS " +
                "user" +
                " (id           INT(11)         NOT NULL," +
                "username       VARCHAR(64)     NOT NULL    UNIQUE," +
                "password       VARCHAR(256)    NULL," +
                "certificate    VARCHAR(512)    NULL        UNIQUE," +
                "last_sync  INT(14)," +
                "PRIMARY KEY (id)" +
                ");");

        db.execSQL(createUser);