Class.forName("com.mysql.jdbc.Driver").newInstance();
con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

Cursor uploadUpdatedUser = db.getUpdated("user");
String userId, userUsername, userPassword, userCertificate;
while(uploadUpdatedUser.moveToNext()) {
    userId = uploadUpdatedUser.getString(0);
    userUsername = uploadUpdatedUser.getString(1);
    userPassword = uploadUpdatedUser.getString(2);
    userCertificate = uploadUpdatedUser.getString(3);

    user = con.prepareStatement("INSERT INTO user (id, username, password, certificate) VALUES(?, ?, ?, ?) " +
        "ON DUPLICATE KEY UPDATE id=VALUES(id), username=VALUES(username), password=VALUES(password), certificate=VALUES(certificate)");
    user.setString(1, userId);
    user.setString(2, userUsername);
    user.setString(3, userPassword);
    user.setString(4, userCertificate);
    user.addBatch();
    Log.i("uploadUpdatedUser", "getting " + userId + userUsername + userPassword + userCertificate + user);
    user.executeBatch();
}