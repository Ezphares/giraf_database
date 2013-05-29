package dk.aau.cs.giraf.puddle;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

import android.os.AsyncTask;
import android.util.Log;

public class DownloadTask extends AsyncTask{

    private static final String TAG = "DownloadTask";
    Database db = null;

    @Override
    protected Object doInBackground(Object... params) {
        Log.i(TAG, "Hello from " + TAG);

        db = MainActivity.db;

        String currentTime = String.valueOf(System.currentTimeMillis());

        String url = "jdbc:mysql://10.0.0.10:3306/giraf";
        String user = "admin";
        String password = "1234";

        try {
            Log.i(TAG, "Trying connection");
            Class.forName("com.mysql.jdbc.Driver").newInstance();
            Connection con = DriverManager.getConnection(url, user, password);
            Statement st = con.createStatement();
            db.insertSync(currentTime);

            ResultSet getUser = st.executeQuery("SELECT * FROM `user`");
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

            ResultSet getDepartment = st.executeQuery("SELECT * FROM `department`");
            String departmentId, departmentName, departmentAddress, departmentPhone, departmentEmail, departmentSuperDepartmentId, departmentAuthor;
            while(getDepartment.next()) {
                departmentId = getDepartment.getString("id");
                departmentName = getDepartment.getString("name");
                departmentAddress = getDepartment.getString("address");
                departmentPhone = getDepartment.getString("phone");
                departmentEmail = getDepartment.getString("email");
                departmentSuperDepartmentId = getDepartment.getString("super_department_id");
                departmentAuthor = getDepartment.getString("author");
                Log.i(TAG, "getDepartment " + departmentId + departmentName + departmentAddress + departmentPhone + departmentEmail + departmentSuperDepartmentId + departmentAuthor + currentTime);
                db.insertDepartment(departmentId, departmentName, departmentAddress, departmentPhone, departmentEmail, departmentSuperDepartmentId, departmentAuthor, currentTime);
            }
            getDepartment.close();

            ResultSet getProfile = st.executeQuery("SELECT * FROM `profile`");
            String profileId, profileName, profilePhone, profilePicture, profileEmail, profileRole, profileAddress, profileSettings, profileUserId, profileDepartmentId, profileAuthor;
            while(getProfile.next()) {
                profileId = getProfile.getString("id");
                profileName = getProfile.getString("name");
                profilePhone = getProfile.getString("phone");
                profilePicture = getProfile.getString("picture");
                profileEmail = getProfile.getString("email");
                profileRole = getProfile.getString("role");
                profileAddress = getProfile.getString("address");
                profileSettings = getProfile.getString("settings");
                profileUserId = getProfile.getString("user_id");
                profileDepartmentId = getProfile.getString("department_id");
                profileAuthor = getProfile.getString("author");
                Log.i(TAG, "getProfile " + profileId + profileName + profilePhone+ profilePicture + profileEmail + profileRole + profileAddress + profileSettings + profileUserId + profileDepartmentId + profileAuthor + currentTime);
                db.insertProfile(profileId, profileName, profilePhone, profilePicture, profileEmail, profileRole, profileAddress, profileSettings, profileUserId, profileDepartmentId, profileAuthor, currentTime);
            }
            getProfile.close();

            ResultSet getPictogram = st.executeQuery("SELECT * FROM `pictogram`");
            String pictogramId, pictogramName, pictogramPublic, pictogramImageData, pictogramSoundData, pictogramInlineText, pictogramAuthor;
            while(getPictogram.next()) {
                pictogramId = getPictogram.getString("id");
                pictogramName = getPictogram.getString("name");
                pictogramPublic = getPictogram.getString("public");
                pictogramImageData = getPictogram.getString("image_data");
                pictogramSoundData = getPictogram.getString("sound_data");
                pictogramInlineText = getPictogram.getString("inline_text");
                pictogramAuthor = getPictogram.getString("author");
                Log.i(TAG, "getCategory " + pictogramId + pictogramName + pictogramPublic+ pictogramImageData + pictogramSoundData + pictogramInlineText + pictogramAuthor + currentTime);
                db.insertPictogram(pictogramId, pictogramName, pictogramPublic, pictogramImageData, pictogramSoundData, pictogramInlineText, pictogramAuthor, currentTime);
            }
            getPictogram.close();

            ResultSet getTag = st.executeQuery("SELECT * FROM `tag`");
            String tagId, tagName;
            while(getTag.next()) {
                tagId = getTag.getString("id");
                tagName = getTag.getString("name");
                Log.i(TAG, "getTag " + tagId + tagName + currentTime);
                db.insertTag(tagId, tagName, currentTime);
            }
            getTag.close();

            ResultSet getCategory = st.executeQuery("SELECT * FROM `category`");
            String categoryId, categoryName, categoryColour, categoryIcon, categorySuperCategoryId;
            while(getCategory.next()) {
                categoryId = getCategory.getString("id");
                categoryName = getCategory.getString("name");
                categoryColour = getCategory.getString("colour");
                categoryIcon = getCategory.getString("icon");
                categorySuperCategoryId = getCategory.getString("super_category_id");
                Log.i(TAG, "getCategory " + categoryId + categoryName + categoryColour+ categoryIcon + categorySuperCategoryId + currentTime);
                db.insertCategory(categoryId, categoryName, categoryColour, categoryIcon, categorySuperCategoryId, currentTime);
            }
            getCategory.close();

            ResultSet getApplication = st.executeQuery("SELECT * FROM `application`");
            String applicationId, applicationName, applicationVersion, applicationIcon, applicationPackage, applicationActivity, applicationDescription, applicationAuthor;
            while(getApplication.next()) {
                applicationId = getApplication.getString("id");
                applicationName = getApplication.getString("name");
                applicationVersion = getApplication.getString("version");
                applicationIcon = getApplication.getString("icon");
                applicationPackage = getApplication.getString("package");
                applicationActivity = getApplication.getString("activity");
                applicationDescription = getApplication.getString("description");
                applicationAuthor = getApplication.getString("author");
                Log.i(TAG, "getApplication " + applicationId + applicationName + applicationVersion+ applicationIcon + applicationPackage + applicationActivity + applicationDescription + applicationAuthor + currentTime);
                db.insertApplication(applicationId, applicationName, applicationVersion, applicationIcon, applicationPackage, applicationActivity, applicationDescription, applicationAuthor, currentTime);
            }
            getApplication.close();

            ResultSet getAdminOf = st.executeQuery("SELECT * FROM `admin_of`");
            String adminOfUserId, adminOfDepartmentId;
            while(getAdminOf.next()) {
                adminOfUserId = getAdminOf.getString("user_id");
                adminOfDepartmentId = getAdminOf.getString("department_id");
                Log.i(TAG, "getAdminOf " + adminOfUserId + adminOfDepartmentId + currentTime);
                db.insertAdminOf(adminOfUserId, adminOfDepartmentId, currentTime);
            }
            getAdminOf.close();

            ResultSet getDepartmentPictogram = st.executeQuery("SELECT * FROM `department_pictogram`");
            String departmentPictogtamDepartmentId, departmentPictogramPictogramId;
            while(getDepartmentPictogram.next()) {
                departmentPictogtamDepartmentId = getDepartmentPictogram.getString("department_id");
                departmentPictogramPictogramId = getDepartmentPictogram.getString("pictogram_id");
                Log.i(TAG, "getDepartmentPictogram " + departmentPictogtamDepartmentId + departmentPictogramPictogramId + currentTime);
                db.insertDepartmentPictogram(departmentPictogtamDepartmentId, departmentPictogramPictogramId, currentTime);
            }
            getDepartmentPictogram.close();

            ResultSet getProfilePictogram = st.executeQuery("SELECT * FROM `profile_pictogram`");
            String profilePictogramProfileId, profilePictogramPictogramId;
            while(getProfilePictogram.next()) {
                profilePictogramProfileId = getProfilePictogram.getString("profile_id");
                profilePictogramPictogramId = getProfilePictogram.getString("pictogram_id");
                Log.i(TAG, "getProfilePictogram " + profilePictogramProfileId + profilePictogramPictogramId + currentTime);
                db.insertProfilePictogram(profilePictogramProfileId, profilePictogramPictogramId, currentTime);
            }
            getProfilePictogram.close();

            ResultSet getDepartmentApplication = st.executeQuery("SELECT * FROM `department_application`");
            String departmentApplicationDepartmentId, departmentApplicationApplicationId;
            while(getDepartmentApplication.next()) {
                departmentApplicationDepartmentId = getDepartmentApplication.getString("department_id");
                departmentApplicationApplicationId = getDepartmentApplication.getString("application_id");
                Log.i(TAG, "getDepartmentApplication " + departmentApplicationDepartmentId + departmentApplicationApplicationId + currentTime);
                db.insertDepartmentApplication(departmentApplicationDepartmentId, departmentApplicationApplicationId, currentTime);
            }
            getDepartmentApplication.close();

            ResultSet getProfileApplication = st.executeQuery("SELECT * FROM `profile_application`");
            String profileApplicationProfileId, profileApplicationApplicationId, profileApplicationSettings;
            while(getProfileApplication.next()) {
                profileApplicationProfileId = getProfileApplication.getString("profile_id");
                profileApplicationApplicationId = getProfileApplication.getString("application_id");
                profileApplicationSettings = getProfileApplication.getString("settings");
                Log.i(TAG, "getProfileApplication " + profileApplicationProfileId + profileApplicationApplicationId + profileApplicationSettings + currentTime);
                db.insertProfileApplication(profileApplicationProfileId, profileApplicationApplicationId, profileApplicationSettings, currentTime);
            }
            getProfileApplication.close();

            ResultSet getPictogramTag = st.executeQuery("SELECT * FROM `pictogram_tag`");
            String pictogramTagPictogramId, pictogramTagTagId;
            while(getPictogramTag.next()) {
                pictogramTagPictogramId = getPictogramTag.getString("pictogram_id");
                pictogramTagTagId = getPictogramTag.getString("tag_id");
                Log.i(TAG, "getPictogramTag " + pictogramTagPictogramId + pictogramTagTagId + currentTime);
                db.insertPictogramTag(pictogramTagPictogramId, pictogramTagTagId, currentTime);
            }
            getPictogramTag.close();

            ResultSet getPictogramCategory = st.executeQuery("SELECT * FROM `pictogram_category`");
            String pictogramCategoryPictogramId, pictogramCategoryCategoryId;
            while(getPictogramCategory.next()) {
                pictogramCategoryPictogramId = getPictogramCategory.getString("pictogram_id");
                pictogramCategoryCategoryId = getPictogramCategory.getString("category_id");
                Log.i(TAG, "getPictogramCategory " + pictogramCategoryPictogramId + pictogramCategoryCategoryId + currentTime);
                db.insertPictogramCategory(pictogramCategoryPictogramId, pictogramCategoryCategoryId, currentTime);
            }
            getPictogramCategory.close();

            ResultSet getProfileCategory = st.executeQuery("SELECT * FROM `profile_category`");
            String profileCategoryProfileId, profileCategoryCategoryId;
            while(getProfileCategory.next()) {
                profileCategoryProfileId = getProfileCategory.getString("profile_id");
                profileCategoryCategoryId = getProfileCategory.getString("category_id");
                Log.i(TAG, "getProfileCategory " + profileCategoryProfileId + profileCategoryCategoryId + currentTime);
                db.insertProfileCategory(profileCategoryProfileId, profileCategoryCategoryId, currentTime);
            }
            getProfileCategory.close();

            ResultSet getGuardianOf = st.executeQuery("SELECT * FROM `guardian_of`");
            String guardianOfGuardianId, guardianOfChildId;
            while(getGuardianOf.next()) {
                guardianOfGuardianId = getGuardianOf.getString("guardian_id");
                guardianOfChildId = getGuardianOf.getString("child_id");
                Log.i(TAG, "getGuardianOf " + guardianOfGuardianId + guardianOfChildId + currentTime);
                db.insertGuardianOf(guardianOfGuardianId, guardianOfChildId, currentTime);
            }
            getGuardianOf.close();

            st.close();
            con.close();
        } catch (Exception e) {
        }

        // TODO Auto-generated method stub
        return null;
    }
}
