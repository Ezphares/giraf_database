Class.forName("com.mysql.jdbc.Driver").newInstance();
Connection con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);
Statement st = con.createStatement();
db.insertSync(currentTime);
Log.i(TAG, "Starting download");

ResultSet downloadUser = st.executeQuery("SELECT * FROM user");
String userId, userUsername, userPassword, userCertificate;
while(downloadUser.next()) {
    userId = downloadUser.getString("id");
    userUsername = downloadUser.getString("username");
    userPassword = downloadUser.getString("password");
    userCertificate = downloadUser.getString("certificate");
    Log.i(TAG, "downloadUser " + userId + userUsername + userPassword + userCertificate + currentTime);
    db.insertUser(userId, userUsername, userPassword, userCertificate, currentTime);
}
downloadUser.close();