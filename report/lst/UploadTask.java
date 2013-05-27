Class.forName("com.mysql.jdbc.Driver").newInstance();
Connection con = DriverManager.getConnection(url, user, password);
Statement st = con.createStatement();

Log.i(TAG, "Uploading changes");
Cursor getUpdatedUser = db.getUpdatedUser();
while(getUpdatedUser.moveToNext()) {
    String userId = getUpdatedUser.getString(0);
    String userUsername = getUpdatedUser.getString(1);
    String userCertificate = getUpdatedUser.getString(3);

    pst = con.prepareStatement("INSERT OR REPLACE INTO user (id, username, password, certificate) VALUES(?, ?, ?, ?)");
    pst.setString(1, userId);
    pst.setString(2, userUsername);
    pst.setString(3, userPassword);
    pst.setString(4, userCertificate);
    pst.addBatch();
    Log.i("getUpdatedUser", "getting " + userId + userUsername + userPassword + userCertificate + pst);
}

pst.executeBatch();