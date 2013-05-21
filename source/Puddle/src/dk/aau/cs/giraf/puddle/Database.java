package dk.aau.cs.giraf.puddle;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class Database {

    private Context context;
    private DbHelper dbHelper;
    private static final String TAG = "Database";

    public Database(Context context) {
        this.context = context;
        dbHelper = new DbHelper();
    }

    public void close(){
        dbHelper.close();
    }

    public void insertUser(String id, String username, String password, String certificate, String lastSync){
        Log.i(TAG, "Inserting user " + id + " " + username + " " + password + " " + certificate + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `user` (id, username, password, certificate, last_sync) " +
                "VALUES ('" + id + "', '" + username + "', '" + password + "', '" + certificate + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertDepartment(String id, String name, String address, String phone, String email, String superDepartmentId, String author, String lastSync){
        Log.i(TAG, "Inserting department " + id + " " + name + " " + address + " " + phone + " " + email + " " + superDepartmentId + " " + author + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `department` (id, name, address, phone, email, super_department_id, author, last_sync) " +
                "VALUES ('" + id + "', '" + name + "', '" + address + "', '" + phone + "', '" + email + "', '" + superDepartmentId + "', '" + author + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertProfile(String id, String name, String phone, String picture, String email, String role, String address, String settings, String userId, String departmentId, String author, String lastSync){
        Log.i(TAG, "Inserting profile " + id + " " + name + " " + phone + " " + picture + " " + email + " " + role + " " + address + " " + settings + " " + userId + " " + departmentId + " " + author + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `profile` (id, name, address, phone, email, super_department_id, author, last_sync) " +
                "VALUES ('" + id + "', '" + name + "', '" + phone + "', '" + picture + "', '" + email + "', '" + role + "', '" + address + "', '" + settings + "', '" + userId + "', '" + departmentId + "', '" + author + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertPictogram(String id, String name, String _public, String imageData, String soundData, String inlineText, String author, String lastSync){
        Log.i(TAG, "Inserting pictogram " + id + " " + name + " " + imageData + " " + soundData + " " + inlineText + " " + author + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `pictogram` (id, name, public, phone, email, super_department_id, author, last_sync) " +
                "VALUES ('" + id + "', '" + name + "', '" + _public + "', '" + imageData + "', '" + soundData + "', '" + inlineText + "', '" + author + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertTag(String id, String name, String lastSync){
        Log.i(TAG, "Inserting tag " + id + " " + name + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `tag` (id, name, last_sync) " +
                "VALUES ('" + id + "', '" + name + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertCategory(String id, String name, String colour, String icon, String superCategoryId, String lastSync){
        Log.i(TAG, "Inserting category " + id + " " + name + " " + icon + " " + superCategoryId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `category` (id, name, colour, icon, super_category_id, last_sync) " +
                "VALUES ('" + id + "', '" + name + "', '" + colour + "', '" + icon + "', '" + superCategoryId + "', '" + lastSync + "')";

        try {
            db.execSQL(sql);
            db.close();
        } catch (Exception e){
        }
    }

    public void insertApplication(String id, String name, String version, String icon, String _package, String activity, String description, String author, String lastSync){
        Log.i(TAG, "Inserting departments " + id + " " + name + " " + version + " " + icon + " " + _package + " " + activity + " " + description + " " + author + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `application` (id, name, version, icon, package, activity, description, author, last_sync) " +
                "VALUES ('" + id + "', '" + name + "', '" + version + "', '" + icon + "', '" + _package + "', '" + activity + "', '" + description + "', '" + author + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertAdminOf(String userId, String departmentId, String lastSync){
        Log.i(TAG, "Inserting admin_of " + userId + " " + departmentId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `admin_of` (user_id, department_id, last_sync) " +
                "VALUES ('" + userId + "', '" + departmentId + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertDepartmentPictogram(String departmentId, String pictogramId, String lastSync){
        Log.i(TAG, "Inserting department_pictogram " + departmentId + " " + pictogramId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `department_pictogram` (department_id, pictogram_id, last_sync) " +
                "VALUES ('" + departmentId + "', '" + pictogramId + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertProfilePictogram(String profileId, String pictogramId, String lastSync){
        Log.i(TAG, "Inserting profile_pictogram " + profileId + " " + pictogramId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `profile_pictogram` (profile_id, pictogram_id, last_sync) " +
                "VALUES ('" + profileId + "', '" + pictogramId + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertDepartmentApplication(String departmentId, String applicationId, String lastSync){
        Log.i(TAG, "Inserting department_application " + departmentId + " " + applicationId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `department_application` (department_id, application_id, last_sync) " +
                "VALUES ('" + departmentId + "', '" + applicationId + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertProfileApplication(String profileId, String applicationId, String settings, String lastSync){
        Log.i(TAG, "Inserting profile_application " + profileId + " " + applicationId + " " + settings + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `profile_application` (department_id, application_id, settings, last_sync) " +
                "VALUES ('" + profileId + "', '" + applicationId + "', '" + settings + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertPictogramTag(String pictogramId, String tagId, String lastSync){
        Log.i(TAG, "Inserting pictogram_tag " + pictogramId + " " + tagId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `pictogram_tag` (pictogram_id, tag_id, last_sync) " +
                "VALUES ('" + pictogramId + "', '" + tagId + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertPictogramCategory(String pictogramId, String categoryId, String lastSync){
        Log.i(TAG, "Inserting pictogram_category " + pictogramId + " " + categoryId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `pictogram_category` (pictogram_id, category_id, last_sync) " +
                "VALUES ('" + pictogramId + "', '" + categoryId + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertProfileCategory(String profileId, String categoryId, String lastSync){
        Log.i(TAG, "Inserting profile_category " + profileId + " " + categoryId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `profile_category` (profile_id, category_id, last_sync) " +
                "VALUES ('" + profileId + "', '" + categoryId + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertGuardianOf(String guardianId, String childId, String lastSync){
        Log.i(TAG, "Inserting guardian_of " + guardianId + " " + childId + " " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `guardian_of` (guardian_id, child_id, last_sync) " +
                "VALUES ('" + guardianId + "', '" + childId + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public void insertSync(String lastSync){
        Log.i(TAG, "Inserting last_sync " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO `last_sync` (id, last_sync) " +
                "VALUES ('" + "1" + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public Cursor getNew() {
        SQLiteDatabase db = dbHelper.getReadableDatabase();

        String getLastSync = "SELECT * FROM last_sync;";

        Cursor cursor = db.rawQuery(getLastSync, null);
        String last_sync = "";
        if(cursor != null) {
            cursor.moveToFirst();
            last_sync = cursor.getString(1);
        }

        Log.i("Get New", "Last sync = " + last_sync);

        String sql = "SELECT * FROM test_table WHERE last_sync > " + last_sync + ";";

        return db.rawQuery(sql, null);

        //Cursor result = db.rawQuery(sql, null);
        //return result;
    }


    public void delete(int id){
    }

    public Cursor query(){
        return null;
    }

    private class DbHelper extends SQLiteOpenHelper {
        public static final String DB_NAME = "giraflocal.sqlite";
        public static final int DB_VERSION = 1;
        public static final String TABLE = "table";
        private static final String TAG = "DbHelper";

        public DbHelper() {
            super(context, DB_NAME, null, DB_VERSION);
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
        	Log.i("Database onCreate", "Creating Database");
            String createUser = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`user`" +
                    " (`id`			INT(11)			NOT NULL," +
                    "`username`		VARCHAR(64)		NOT NULL	UNIQUE," +
                    "`password`		VARCHAR(256)	NULL," +
                    "`certificate`	VARCHAR(512)	NULL		UNIQUE," +
                    "`last_sync`	INT(14)," +
                    "PRIMARY KEY (`id`)" +
                    ");");

            String createDepartment = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`department`" +
                    " (`id`					INT(11)			NOT NULL," +
                    "`name`					VARCHAR(64)		NOT NULL," +
                    "`address`				VARCHAR(256)	NOT NULL," +
                    "`phone`				INT(11)			NOT NULL," +
                    "`email`				VARCHAR(64)		NOT NULL," +
                    "`super_department_id`	INT(11)			NULL," +
                    "`author`				INT(11)			NULL," +
                    "`last_sync`			INT(14)," +
                    "PRIMARY KEY (`id`)," +
                    "FOREIGN KEY (`super_department_id`) REFERENCES `department` (`id`)	ON DELETE SET NULL," +
                    "FOREIGN KEY (`author`) REFERENCES `user` (`id`) ON DELETE SET NULL" +
                    ");");

            String createProfile = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`profile`" +
                    " (`id`				INT(11)			NOT NULL," +
                    "`name`				VARCHAR(64)		NOT NULL," +
                    "`phone`			VARCHAR(11)		NULL," +
                    "`picture`			BLOB			NULL," +
                    "`email`			VARCHAR(64)		NULL," +
                    "`role`				SMALLINT		NOT NULL," +
                    "`address`			VARCHAR(256)	NOT NULL," +
                    "`settings`			BLOB			NULL," +
                    "`user_id`			INT(11)			NULL		UNIQUE," +
                    "`department_id`	INT(11)			NOT NULL," +
                    "`author`           INT(11)         NULL," +
                    "`last_sync`		INT(14)," +
                    "PRIMARY KEY (`id`)," +
                    "FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE SET NULL," +
                    "FOREIGN KEY (`department_id`) REFERENCES `department` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`author`) REFERENCES `profile` (`id`) ON DELETE SET NULL" +
                    ");");

            String createPictogram = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`pictogram`" +
                    " (`id`			INT(11)			NOT NULL," +
                    "`name`			VARCHAR(64)		NOT NULL," +
                    "`public`		TINYINT			NOT NULL," +
                    "`image_data`	BLOB			NULL," +
                    "`sound_data`	BLOB			NULL," +
                    "`inline_text`	VARCHAR(64)		NULL," +
                    "`author`		INT(11)			NULL," +
                    "`last_sync`	INT(14)," +
                    "PRIMARY KEY (`id`)," +
                    "FOREIGN KEY (`author`) REFERENCES `user` (`id`) ON DELETE SET NULL" +
                    ");");

            String createTag = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`tag`" +
                    " (`id`			INT(11)			NOT NULL," +
                    "`name`			VARCHAR(64)		NOT NULL," +
                    "`last_sync`	INT(14)," +
                    "PRIMARY KEY (`id`)" +
                    ");");

            String createCategory = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`category`" +
                    " (`id`					INT(11)			NOT NULL," +
                    "`name`					VARCHAR(64)		NOT NULL," +
                    "`colour`				VARCHAR(11)		NOT NULL," +
                    "`icon`					BLOB			NULL," +
                    "`super_category_id`	INT(11)			NULL," +
                    "`last_sync`			INT(14)," +
                    "PRIMARY KEY (`id`)," +
                    "FOREIGN KEY (`super_category_id`) REFERENCES `category` (`id`)	ON DELETE SET NULL" +
                    ");");

            String createApplication = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`application`" +
                    " (`id`			INT(11)			NOT NULL," +
                    "`name`			VARCHAR(64) 	NOT NULL," +
                    "`version`		VARCHAR(32)		NOT NULL," +
                    "`icon`			BLOB			NOT NULL," +
                    "`package`		VARCHAR(256)	NOT NULL," +
                    "`activity`		VARCHAR(64)		NOT NULL," +
                    "`description`	VARCHAR(1024)	NOT NULL," +
                    "`author`		INT(11)			NULL," +
                    "`last_sync`	INT(14)," +
                    "PRIMARY KEY (`id`)," +
                    "FOREIGN KEY (`author`) REFERENCES `user` (`id`) ON DELETE SET NULL" +
                    ");");

            String createAdminOf = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`admin_of`" +
                    " (`user_id`			INT(11)		NOT NULL," +
                    "`department_id`		INT(11)		NOT NULL," +
                    "`last_sync`			INT(14)," +
                    "PRIMARY KEY (`user_id`, `department_id`)," +
                    "FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`department_id`) REFERENCES `department` (`id`) ON DELETE CASCADE" +
                    ");");

            String createDepartmentPictogram = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`department_pictogram`" +
                    " (`department_id`		INT(11)		NOT NULL," +
                    "`pictogram_id`			INT(11)		NOT NULL," +
                    "`last_sync`			INT(14)," +
                    "FOREIGN KEY (`department_id`) REFERENCES `department` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`pictogram_id`) REFERENCES `pictogram` (`id`) ON DELETE CASCADE" +
                    ");");

            String createProfilePictogram = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`profile_pictogram`" +
                    " (`profile_id`		INT(11)		NOT NULL," +
                    "`pictogram_id`		INT(11)		NOT NULL," +
                    "`last_sync`		INT(14)," +
                    "FOREIGN KEY (`profile_id`) REFERENCES `profile` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`pictogram_id`) REFERENCES `pictogram` (`id`) ON DELETE CASCADE" +
                    ");");

            String createDepartmentApplication = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`department_application`" +
                    " (`department_id`		INT(11)		NOT NULL," +
                    "`application_id`		INT(11)		NOT NULL," +
                    "`last_sync`			INT(14)," +
                    "FOREIGN KEY (`department_id`) REFERENCES `department` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`application_id`) REFERENCES `application` (`id`) ON DELETE CASCADE" +
                    ");");

            String createProfileApplication = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`profile_application`" +
                    " (`profile_id`		INT(11)		NOT NULL," +
                    "`application_id`	INT(11)		NOT NULL," +
                    "`settings`			BLOB		NULL," +
                    "`last_sync`		INT(14)," +
                    "FOREIGN KEY (`profile_id`) REFERENCES `profile` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`application_id`) REFERENCES `application` (`id`) ON DELETE CASCADE" +
                    ");");

            String createPictogramTag = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`pictogram_tag`" +
                    " (`pictogram_id`		INT(11)		NOT NULL," +
                    "`tag_id`				INT(11)		NOT NULL," +
                    "`last_sync`			INT(14)," +
                    "FOREIGN KEY (`pictogram_id`) REFERENCES `pictogram` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`tag_id`) REFERENCES `tag` (`id`) ON DELETE CASCADE" +
                    ");");

            String createPictogramCategory = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`pictogram_category`" +
                    " (``pictogram_id`		INT(11)		NOT NULL," +
                    "`category_id`			INT(11)		NOT NULL," +
                    "`last_sync`			INT(14)," +
                    "FOREIGN KEY (`pictogram_id`) REFERENCES `pictogram` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`category_id`) REFERENCES `category` (`id`) ON DELETE CASCADE" +
                    ");");

            String createProfileCategory = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`profile_category`" +
                    " (`profile_id`		INT(11)		NOT NULL," +
                    "`category_id`		INT(11)		NOT NULL," +
                    "`last_sync`		INT(14)," +
                    "FOREIGN KEY (`profile_id`) REFERENCES `profile` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`category_id`) REFERENCES `category` (`id`) ON DELETE CASCADE" +
                    ");");

            String createGuardianOf = String.format("CREATE TABLE IF NOT EXISTS " +
                    "`guardian_of`" +
                    " (`guardian_id`	INT(11)		NOT NULL," +
                    "`child_id`			INT(11)		NOT NULL," +
                    "`last_sync`		INT(14)," +
                    "FOREIGN KEY (`guardian_id`) REFERENCES `profile` (`id`) ON DELETE CASCADE," +
                    "FOREIGN KEY (`child_id`) REFERENCES `profile` (`id`) ON DELETE CASCADE" +
                    ");");

            String createLastSync = String.format("CREATE TABLE IF NOT EXISTS " +
                    "last_sync" +
                    " (id INT(11) UNIQUE," +
                    "last_sync INT(14)" +
                    ");");

            db.execSQL(createUser);
            db.execSQL(createDepartment);
            db.execSQL(createProfile);
            db.execSQL(createPictogram);
            db.execSQL(createTag);
            db.execSQL(createCategory);
            db.execSQL(createApplication);
            db.execSQL(createAdminOf);
            db.execSQL(createDepartmentPictogram);
            db.execSQL(createProfilePictogram);
            db.execSQL(createDepartmentApplication);
            db.execSQL(createProfileApplication);
            db.execSQL(createPictogramTag);
            db.execSQL(createPictogramCategory);
            db.execSQL(createProfileCategory);
            db.execSQL(createGuardianOf);
            db.execSQL(createLastSync);
        }


        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            db.execSQL("DROP TABLE IF EXISTS " + TABLE);
            Log.d(TAG, "onUpdate dropped table " + TABLE);
            this.onCreate(db);
        }
    }
}