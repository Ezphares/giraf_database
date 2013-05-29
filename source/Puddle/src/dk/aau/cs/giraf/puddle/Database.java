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

<<<<<<< HEAD
    /** Insert functions inserts data into the tables in the local database */
    public void insertUser(String id, String username, String password, String certificate, String timestamp){
        Log.i(TAG, "Inserting user " + id + " " + username + " " + password + " " + certificate + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO user (id, username, password, certificate, timestamp) " +
                "VALUES ('" + id + "', '" + username + "', '" + password + "', '" + certificate + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertDepartment(String id, String name, String address, String phone, String email, String superDepartmentId, String author, String timestamp){
        Log.i(TAG, "Inserting department " + id + " " + name + " " + address + " " + phone + " " + email + " " + superDepartmentId + " " + author + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO department (id, name, address, phone, email, super_department_id, author, timestamp) " +
                "VALUES ('" + id + "', '" + name + "', '" + address + "', '" + phone + "', '" + email + "', '" + superDepartmentId + "', '" + author + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertProfile(String id, String name, String phone, String picture, String email, String role, String address, String settings, String userId, String departmentId, String author, String timestamp){
        Log.i(TAG, "Inserting profile " + id + " " + name + " " + phone + " " + picture + " " + email + " " + role + " " + address + " " + settings + " " + userId + " " + departmentId + " " + author + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO profile (id, name, phone, picture, email, role, address, settings, user_id, department_id, author, timestamp) " +
                "VALUES ('" + id + "', '" + name + "', '" + phone + "', '" + picture + "', '" + email + "', '" + role + "', '" + address + "', '" + settings + "', '" + userId + "', '" + departmentId + "', '" + author + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertPictogram(String id, String name, String _public, String imageData, String soundData, String inlineText, String author, String timestamp){
        Log.i(TAG, "Inserting pictogram " + id + " " + name + " " + imageData + " " + soundData + " " + inlineText + " " + author + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO pictogram (id, name, public, image_data, sound_data, inline_text, author, timestamp) " +
                "VALUES ('" + id + "', '" + name + "', '" + _public + "', '" + imageData + "', '" + soundData + "', '" + inlineText + "', '" + author + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertTag(String id, String name, String timestamp){
        Log.i(TAG, "Inserting tag " + id + " " + name + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO tag (id, name, timestamp) " +
                "VALUES ('" + id + "', '" + name + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertCategory(String id, String name, String colour, String icon, String superCategoryId, String timestamp){
        Log.i(TAG, "Inserting category " + id + " " + name + " " + icon + " " + superCategoryId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO category (id, name, colour, icon, super_category_id, timestamp) " +
                "VALUES ('" + id + "', '" + name + "', '" + colour + "', '" + icon + "', '" + superCategoryId + "', '" + timestamp + "')";

            db.execSQL(sql);
    }

    public void insertApplication(String id, String name, String version, String icon, String _package, String activity, String description, String author, String timestamp){
        Log.i(TAG, "Inserting departments " + id + " " + name + " " + version + " " + icon + " " + _package + " " + activity + " " + description + " " + author + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO application (id, name, version, icon, package, activity, description, author, timestamp) " +
                "VALUES ('" + id + "', '" + name + "', '" + version + "', '" + icon + "', '" + _package + "', '" + activity + "', '" + description + "', '" + author + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertAdminOf(String userId, String departmentId, String timestamp){
        Log.i(TAG, "Inserting admin_of " + userId + " " + departmentId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO admin_of (user_id, department_id, timestamp) " +
                "VALUES ('" + userId + "', '" + departmentId + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertDepartmentPictogram(String departmentId, String pictogramId, String timestamp){
        Log.i(TAG, "Inserting department_pictogram " + departmentId + " " + pictogramId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO department_pictogram (department_id, pictogram_id, timestamp) " +
                "VALUES ('" + departmentId + "', '" + pictogramId + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertProfilePictogram(String profileId, String pictogramId, String timestamp){
        Log.i(TAG, "Inserting profile_pictogram " + profileId + " " + pictogramId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO profile_pictogram (profile_id, pictogram_id, timestamp) " +
                "VALUES ('" + profileId + "', '" + pictogramId + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertDepartmentApplication(String departmentId, String applicationId, String timestamp){
        Log.i(TAG, "Inserting department_application " + departmentId + " " + applicationId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO department_application (department_id, application_id, timestamp) " +
                "VALUES ('" + departmentId + "', '" + applicationId + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertProfileApplication(String profileId, String applicationId, String settings, String timestamp){
        Log.i(TAG, "Inserting profile_application " + profileId + " " + applicationId + " " + settings + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO profile_application (department_id, application_id, settings, timestamp) " +
                "VALUES ('" + profileId + "', '" + applicationId + "', '" + settings + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertPictogramTag(String pictogramId, String tagId, String timestamp){
        Log.i(TAG, "Inserting pictogram_tag " + pictogramId + " " + tagId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO pictogram_tag (pictogram_id, tag_id, timestamp) " +
                "VALUES ('" + pictogramId + "', '" + tagId + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertPictogramCategory(String pictogramId, String categoryId, String timestamp){
        Log.i(TAG, "Inserting pictogram_category " + pictogramId + " " + categoryId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO pictogram_category (pictogram_id, category_id, timestamp) " +
                "VALUES ('" + pictogramId + "', '" + categoryId + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertProfileCategory(String profileId, String categoryId, String timestamp){
        Log.i(TAG, "Inserting profile_category " + profileId + " " + categoryId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO profile_category (profile_id, category_id, timestamp) " +
                "VALUES ('" + profileId + "', '" + categoryId + "', '" + timestamp + "')";

        db.execSQL(sql);
    }

    public void insertGuardianOf(String guardianId, String childId, String timestamp){
        Log.i(TAG, "Inserting guardian_of " + guardianId + " " + childId + " " + timestamp);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

        String sql = "INSERT OR REPLACE INTO guardian_of (guardian_id, child_id, timestamp) " +
                "VALUES ('" + guardianId + "', '" + childId + "', '" + timestamp + "')";

        db.execSQL(sql);
=======
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
>>>>>>> dev
    }

    public void insertSync(String lastSync){
        Log.i(TAG, "Inserting last_sync " + lastSync);

        SQLiteDatabase db = dbHelper.getWritableDatabase();

<<<<<<< HEAD
        String sql = "INSERT OR REPLACE INTO last_sync (id, last_sync) " +
                "VALUES ('" + "1" + "', '" + lastSync + "')";

        db.execSQL(sql);
    }

    /** Returns rows newer than the time of last synchronization */
    public Cursor getUpdated(String table) {
=======
        String sql = "INSERT OR REPLACE INTO `last_sync` (id, last_sync) " +
                "VALUES ('" + "1" + "', '" + lastSync + "')";

        db.execSQL(sql);
        db.close();
    }

    public Cursor getNew() {
>>>>>>> dev
        SQLiteDatabase db = dbHelper.getReadableDatabase();

        String getLastSync = "SELECT * FROM last_sync;";

        Cursor cursor = db.rawQuery(getLastSync, null);
        String last_sync = "";
        if(cursor != null) {
            cursor.moveToFirst();
            last_sync = cursor.getString(1);
        }

<<<<<<< HEAD
        Log.i("Get Updated", "Last sync = " + last_sync);

        String sql = "SELECT * FROM " + table + " WHERE timestamp > " + last_sync + ";";

        return db.rawQuery(sql, null);
    }

    /** Creates the database when the database is first accessed. */
    private class DbHelper extends SQLiteOpenHelper {
        public static final String DB_NAME = "giraflocal.sqlite";
        public static final int DB_VERSION = 1;
=======
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
>>>>>>> dev
        private static final String TAG = "DbHelper";

        public DbHelper() {
            super(context, DB_NAME, null, DB_VERSION);
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
<<<<<<< HEAD
        	Log.i(TAG, "Creating Database");
            String createUser = String.format("CREATE TABLE IF NOT EXISTS " +
                    "user" +
                    " (id			INT(11)			NOT NULL," +
                    "username		VARCHAR(64)		NOT NULL	UNIQUE," +
                    "password		VARCHAR(256)	NULL," +
                    "certificate	VARCHAR(512)	NULL		UNIQUE," +
                    "timestamp	    INT(14)," +
                    "PRIMARY KEY (id)" +
                    ");");

            String createDepartment = String.format("CREATE TABLE IF NOT EXISTS " +
                    "department" +
                    " (id					INT(11)			NOT NULL," +
                    "name					VARCHAR(64)		NOT NULL," +
                    "address				VARCHAR(256)	NOT NULL," +
                    "phone				INT(11)			NOT NULL," +
                    "email				VARCHAR(64)		NOT NULL," +
                    "super_department_id	INT(11)			NULL," +
                    "author				INT(11)			NULL," +
                    "timestamp			INT(14)," +
                    "PRIMARY KEY (id)," +
                    "FOREIGN KEY (super_department_id) REFERENCES department (id) ON DELETE SET NULL," +
                    "FOREIGN KEY (author) REFERENCES user (id) ON DELETE SET NULL" +
                    ");");

            String createProfile = String.format("CREATE TABLE IF NOT EXISTS " +
                    "profile" +
                    " (id				INT(11)			NOT NULL," +
                    "name				VARCHAR(64)		NOT NULL," +
                    "phone			VARCHAR(11)		NULL," +
                    "picture			BLOB			NULL," +
                    "email			VARCHAR(64)		NULL," +
                    "role				SMALLINT		NOT NULL," +
                    "address			VARCHAR(256)	NOT NULL," +
                    "settings			BLOB			NULL," +
                    "user_id			INT(11)			NULL		UNIQUE," +
                    "department_id	INT(11)			NOT NULL," +
                    "author           INT(11)         NULL," +
                    "timestamp		INT(14)," +
                    "PRIMARY KEY (id)," +
                    "FOREIGN KEY (user_id) REFERENCES user (id) ON DELETE SET NULL," +
                    "FOREIGN KEY (department_id) REFERENCES department (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (author) REFERENCES profile (id) ON DELETE SET NULL" +
                    ");");

            String createPictogram = String.format("CREATE TABLE IF NOT EXISTS " +
                    "pictogram" +
                    " (id			INT(11)			NOT NULL," +
                    "name			VARCHAR(64)		NOT NULL," +
                    "public		TINYINT			NOT NULL," +
                    "image_data	BLOB			NULL," +
                    "sound_data	BLOB			NULL," +
                    "inline_text	VARCHAR(64)		NULL," +
                    "author		INT(11)			NULL," +
                    "timestamp	INT(14)," +
                    "PRIMARY KEY (id)," +
                    "FOREIGN KEY (author) REFERENCES user (id) ON DELETE SET NULL" +
                    ");");

            String createTag = String.format("CREATE TABLE IF NOT EXISTS " +
                    "tag" +
                    " (id			INT(11)			NOT NULL," +
                    "name			VARCHAR(64)		NOT NULL," +
                    "timestamp   	INT(14)," +
                    "PRIMARY KEY (id)" +
                    ");");

            String createCategory = String.format("CREATE TABLE IF NOT EXISTS " +
                    "category" +
                    " (id					INT(11)			NOT NULL," +
                    "name					VARCHAR(64)		NOT NULL," +
                    "colour				VARCHAR(11)		NOT NULL," +
                    "icon					BLOB			NULL," +
                    "super_category_id	INT(11)			NULL," +
                    "timestamp			INT(14)," +
                    "PRIMARY KEY (id)," +
                    "FOREIGN KEY (super_category_id) REFERENCES category (id)	ON DELETE SET NULL" +
                    ");");

            String createApplication = String.format("CREATE TABLE IF NOT EXISTS " +
                    "application" +
                    " (id         INT(11)         NOT NULL," +
                    "name         VARCHAR(64)     NOT NULL," +
                    "version      VARCHAR(32)     NOT NULL," +
                    "icon         BLOB            NOT NULL," +
                    "package      VARCHAR(256)    NOT NULL," +
                    "activity     VARCHAR(64)     NOT NULL," +
                    "description  VARCHAR(1024)   NOT NULL," +
                    "author       INT(11)         NULL," +
                    "timestamp    INT(14)," +
                    "PRIMARY KEY (id)," +
                    "FOREIGN KEY (author) REFERENCES user (id) ON DELETE SET NULL" +
                    ");");

            String createAdminOf = String.format("CREATE TABLE IF NOT EXISTS " +
                    "admin_of" +
                    " (user_id            INT(11)     NOT NULL," +
                    "department_id        INT(11)     NOT NULL," +
                    "timestamp             INT(14)," +
                    "PRIMARY KEY (user_id, department_id)," +
                    "FOREIGN KEY (user_id) REFERENCES user (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (department_id) REFERENCES department (id) ON DELETE CASCADE" +
                    ");");

            String createDepartmentPictogram = String.format("CREATE TABLE IF NOT EXISTS " +
                    "department_pictogram" +
                    " (department_id      INT(11)     NOT NULL," +
                    "pictogram_id         INT(11)     NOT NULL," +
                    "timestamp            INT(14)," +
                    "FOREIGN KEY (department_id) REFERENCES department (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (pictogram_id) REFERENCES pictogram (id) ON DELETE CASCADE" +
                    ");");

            String createProfilePictogram = String.format("CREATE TABLE IF NOT EXISTS " +
                    "profile_pictogram" +
                    " (profile_id     INT(11)     NOT NULL," +
                    "pictogram_id     INT(11)     NOT NULL," +
                    "timestamp        INT(14)," +
                    "FOREIGN KEY (profile_id) REFERENCES profile (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (pictogram_id) REFERENCES pictogram (id) ON DELETE CASCADE" +
                    ");");

            String createDepartmentApplication = String.format("CREATE TABLE IF NOT EXISTS " +
                    "department_application" +
                    " (department_id      INT(11)     NOT NULL," +
                    "application_id       INT(11)     NOT NULL," +
                    "timestamp            INT(14)," +
                    "FOREIGN KEY (department_id) REFERENCES department (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (application_id) REFERENCES application (id) ON DELETE CASCADE" +
                    ");");

            String createProfileApplication = String.format("CREATE TABLE IF NOT EXISTS " +
                    "profile_application" +
                    " (profile_id     INT(11)     NOT NULL," +
                    "application_id   INT(11)     NOT NULL," +
                    "settings         BLOB        NULL," +
                    "timestamp        INT(14)," +
                    "FOREIGN KEY (profile_id) REFERENCES profile (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (application_id) REFERENCES application (id) ON DELETE CASCADE" +
                    ");");

            String createPictogramTag = String.format("CREATE TABLE IF NOT EXISTS " +
                    "pictogram_tag" +
                    " (pictogram_id       INT(11)     NOT NULL," +
                    "tag_id               INT(11)     NOT NULL," +
                    "timestamp            INT(14)," +
                    "FOREIGN KEY (pictogram_id) REFERENCES pictogram (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (tag_id) REFERENCES tag (id) ON DELETE CASCADE" +
                    ");");

            String createPictogramCategory = String.format("CREATE TABLE IF NOT EXISTS " +
                    "pictogram_category" +
                    " (pictogram_id      INT(11)     NOT NULL," +
                    "category_id          INT(11)     NOT NULL," +
                    "timestamp            INT(14)," +
                    "FOREIGN KEY (pictogram_id) REFERENCES pictogram (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (category_id) REFERENCES category (id) ON DELETE CASCADE" +
                    ");");

            String createProfileCategory = String.format("CREATE TABLE IF NOT EXISTS " +
                    "profile_category" +
                    " (profile_id     INT(11)     NOT NULL," +
                    "category_id      INT(11)     NOT NULL," +
                    "timestamp        INT(14)," +
                    "FOREIGN KEY (profile_id) REFERENCES profile (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (category_id) REFERENCES category (id) ON DELETE CASCADE" +
                    ");");

            String createGuardianOf = String.format("CREATE TABLE IF NOT EXISTS " +
                    "guardian_of" +
                    " (guardian_id    INT(11)     NOT NULL," +
                    "child_id         INT(11)     NOT NULL," +
                    "timestamp        INT(14)," +
                    "FOREIGN KEY (guardian_id) REFERENCES profile (id) ON DELETE CASCADE," +
                    "FOREIGN KEY (child_id) REFERENCES profile (id) ON DELETE CASCADE" +
=======
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
>>>>>>> dev
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
<<<<<<< HEAD
            //Needed for class SQLiteOpenHelper to be valid, currently not used.
=======
            db.execSQL("DROP TABLE IF EXISTS " + TABLE);
            Log.d(TAG, "onUpdate dropped table " + TABLE);
            this.onCreate(db);
>>>>>>> dev
        }
    }
}