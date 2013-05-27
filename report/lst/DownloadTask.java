Class.forName("com.mysql.jdbc.Driver").newInstance();
Connection con = DriverManager.getConnection(url, user, password);
Statement st = con.createStatement();
db.insertSync(currentTime);
Log.i(TAG, "Starting download");
ResultSet getUser = st.executeQuery("SELECT * FROM user");
String userId, userUsername, userPassword, userCertificate;
while(getUser.next()) {
    userId = getUser.getString("id");
    userUsername = getUser.getString("username");
    userPassword = getUser.getString("password");
    userCertificate = getUser.getString("certificate");
    Log.i(TAG, "getUsers " + userId + userUsername + userPassword + userCertificate + currentTime);
    db.insertUser(userId, userUsername, userPassword, userCertificate, currentTime);
}
getUser.close();