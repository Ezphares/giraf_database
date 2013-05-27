public class PuddleApplication extends Application{
    
    private Database db = null;

    @Override
    public void onCreate() {
        super.onCreate();
        setDatabase(new Database(this));
    }

    public Database getDatabase() {
        return db;
    }

    public void setDatabase(Database db) {
        this.db = db;
    }
}