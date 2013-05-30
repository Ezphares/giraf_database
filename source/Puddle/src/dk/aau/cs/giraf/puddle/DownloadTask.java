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

        //Gets current time in milliseconds, used for timestamp in database.
        String currentTime = String.valueOf(System.currentTimeMillis());

        try {
            Log.i(TAG, "Trying connection");
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

            ResultSet downloadDepartment = st.executeQuery("SELECT * FROM department");
            String departmentId, departmentName, departmentAddress, departmentPhone, departmentEmail, departmentSuperDepartmentId, departmentAuthor;
            while(downloadDepartment.next()) {
                departmentId = downloadDepartment.getString("id");
                departmentName = downloadDepartment.getString("name");
                departmentAddress = downloadDepartment.getString("address");
                departmentPhone = downloadDepartment.getString("phone");
                departmentEmail = downloadDepartment.getString("email");
                departmentSuperDepartmentId = downloadDepartment.getString("super_department_id");
                departmentAuthor = downloadDepartment.getString("author");
                Log.i(TAG, "downloadDepartment " + departmentId + departmentName + departmentAddress + departmentPhone + departmentEmail + departmentSuperDepartmentId + departmentAuthor + currentTime);
                db.insertDepartment(departmentId, departmentName, departmentAddress, departmentPhone, departmentEmail, departmentSuperDepartmentId, departmentAuthor, currentTime);
            }
            downloadDepartment.close();

            ResultSet downloadProfile = st.executeQuery("SELECT * FROM profile");
            String profileId, profileName, profilePhone, profilePicture, profileEmail, profileRole, profileAddress, profileSettings, profileUserId, profileDepartmentId, profileAuthor;
            while(downloadProfile.next()) {
                profileId = downloadProfile.getString("id");
                profileName = downloadProfile.getString("name");
                profilePhone = downloadProfile.getString("phone");
                profilePicture = downloadProfile.getString("picture");
                profileEmail = downloadProfile.getString("email");
                profileRole = downloadProfile.getString("role");
                profileAddress = downloadProfile.getString("address");
                profileSettings = downloadProfile.getString("settings");
                profileUserId = downloadProfile.getString("user_id");
                profileDepartmentId = downloadProfile.getString("department_id");
                profileAuthor = downloadProfile.getString("author");
                Log.i(TAG, "downloadProfile " + profileId + profileName + profilePhone+ profilePicture + profileEmail + profileRole + profileAddress + profileSettings + profileUserId + profileDepartmentId + profileAuthor + currentTime);
                db.insertProfile(profileId, profileName, profilePhone, profilePicture, profileEmail, profileRole, profileAddress, profileSettings, profileUserId, profileDepartmentId, profileAuthor, currentTime);
            }
            downloadProfile.close();

            ResultSet downloadPictogram = st.executeQuery("SELECT * FROM pictogram");
            String pictogramId, pictogramName, pictogramPublic, pictogramImageData, pictogramSoundData, pictogramInlineText, pictogramAuthor;
            while(downloadPictogram.next()) {
                pictogramId = downloadPictogram.getString("id");
                pictogramName = downloadPictogram.getString("name");
                pictogramPublic = downloadPictogram.getString("public");
                pictogramImageData = downloadPictogram.getString("image_data");
                pictogramSoundData = downloadPictogram.getString("sound_data");
                pictogramInlineText = downloadPictogram.getString("inline_text");
                pictogramAuthor = downloadPictogram.getString("author");
                Log.i(TAG, "downloadPictogram " + pictogramId + pictogramName + pictogramPublic+ pictogramImageData + pictogramSoundData + pictogramInlineText + pictogramAuthor + currentTime);
                db.insertPictogram(pictogramId, pictogramName, pictogramPublic, pictogramImageData, pictogramSoundData, pictogramInlineText, pictogramAuthor, currentTime);
            }
            downloadPictogram.close();

            ResultSet downloadTag = st.executeQuery("SELECT * FROM tag");
            String tagId, tagName;
            while(downloadTag.next()) {
                tagId = downloadTag.getString("id");
                tagName = downloadTag.getString("name");
                Log.i(TAG, "downloadTag " + tagId + tagName + currentTime);
                db.insertTag(tagId, tagName, currentTime);
            }
            downloadTag.close();

            ResultSet downloadCategory = st.executeQuery("SELECT * FROM category");
            String categoryId, categoryName, categoryColour, categoryIcon, categorySuperCategoryId;
            while(downloadCategory.next()) {
                categoryId = downloadCategory.getString("id");
                categoryName = downloadCategory.getString("name");
                categoryColour = downloadCategory.getString("colour");
                categoryIcon = downloadCategory.getString("icon");
                categorySuperCategoryId = downloadCategory.getString("super_category_id");
                Log.i(TAG, "downloadCategory " + categoryId + categoryName + categoryColour+ categoryIcon + categorySuperCategoryId + currentTime);
                db.insertCategory(categoryId, categoryName, categoryColour, categoryIcon, categorySuperCategoryId, currentTime);
            }
            downloadCategory.close();

            ResultSet downloadApplication = st.executeQuery("SELECT * FROM application");
            String applicationId, applicationName, applicationVersion, applicationIcon, applicationPackage, applicationActivity, applicationDescription, applicationAuthor;
            while(downloadApplication.next()) {
                applicationId = downloadApplication.getString("id");
                applicationName = downloadApplication.getString("name");
                applicationVersion = downloadApplication.getString("version");
                applicationIcon = downloadApplication.getString("icon");
                applicationPackage = downloadApplication.getString("package");
                applicationActivity = downloadApplication.getString("activity");
                applicationDescription = downloadApplication.getString("description");
                applicationAuthor = downloadApplication.getString("author");
                Log.i(TAG, "downloadApplication " + applicationId + applicationName + applicationVersion+ applicationIcon + applicationPackage + applicationActivity + applicationDescription + applicationAuthor + currentTime);
                db.insertApplication(applicationId, applicationName, applicationVersion, applicationIcon, applicationPackage, applicationActivity, applicationDescription, applicationAuthor, currentTime);
            }
            downloadApplication.close();

            ResultSet downloadAdminOf = st.executeQuery("SELECT * FROM admin_of");
            String adminOfUserId, adminOfDepartmentId;
            while(downloadAdminOf.next()) {
                adminOfUserId = downloadAdminOf.getString("user_id");
                adminOfDepartmentId = downloadAdminOf.getString("department_id");
                Log.i(TAG, "downloadAdminOf " + adminOfUserId + adminOfDepartmentId + currentTime);
                db.insertAdminOf(adminOfUserId, adminOfDepartmentId, currentTime);
            }
            downloadAdminOf.close();

            ResultSet downloadDepartmentPictogram = st.executeQuery("SELECT * FROM department_pictogram");
            String departmentPictogtamDepartmentId, departmentPictogramPictogramId;
            while(downloadDepartmentPictogram.next()) {
                departmentPictogtamDepartmentId = downloadDepartmentPictogram.getString("department_id");
                departmentPictogramPictogramId = downloadDepartmentPictogram.getString("pictogram_id");
                Log.i(TAG, "downloadDepartmentPictogram " + departmentPictogtamDepartmentId + departmentPictogramPictogramId + currentTime);
                db.insertDepartmentPictogram(departmentPictogtamDepartmentId, departmentPictogramPictogramId, currentTime);
            }
            downloadDepartmentPictogram.close();

            ResultSet downloadProfilePictogram = st.executeQuery("SELECT * FROM profile_pictogram");
            String profilePictogramProfileId, profilePictogramPictogramId;
            while(downloadProfilePictogram.next()) {
                profilePictogramProfileId = downloadProfilePictogram.getString("profile_id");
                profilePictogramPictogramId = downloadProfilePictogram.getString("pictogram_id");
                Log.i(TAG, "downloadProfilePictogram " + profilePictogramProfileId + profilePictogramPictogramId + currentTime);
                db.insertProfilePictogram(profilePictogramProfileId, profilePictogramPictogramId, currentTime);
            }
            downloadProfilePictogram.close();

            ResultSet downloadDepartmentApplication = st.executeQuery("SELECT * FROM department_application");
            String departmentApplicationDepartmentId, departmentApplicationApplicationId;
            while(downloadDepartmentApplication.next()) {
                departmentApplicationDepartmentId = downloadDepartmentApplication.getString("department_id");
                departmentApplicationApplicationId = downloadDepartmentApplication.getString("application_id");
                Log.i(TAG, "downloadDepartmentApplication " + departmentApplicationDepartmentId + departmentApplicationApplicationId + currentTime);
                db.insertDepartmentApplication(departmentApplicationDepartmentId, departmentApplicationApplicationId, currentTime);
            }
            downloadDepartmentApplication.close();

            ResultSet downloadProfileApplication = st.executeQuery("SELECT * FROM profile_application");
            String profileApplicationProfileId, profileApplicationApplicationId, profileApplicationSettings;
            while(downloadProfileApplication.next()) {
                profileApplicationProfileId = downloadProfileApplication.getString("profile_id");
                profileApplicationApplicationId = downloadProfileApplication.getString("application_id");
                profileApplicationSettings = downloadProfileApplication.getString("settings");
                Log.i(TAG, "downloadProfileApplication " + profileApplicationProfileId + profileApplicationApplicationId + profileApplicationSettings + currentTime);
                db.insertProfileApplication(profileApplicationProfileId, profileApplicationApplicationId, profileApplicationSettings, currentTime);
            }
            downloadProfileApplication.close();

            ResultSet downloadPictogramTag = st.executeQuery("SELECT * FROM pictogram_tag");
            String pictogramTagPictogramId, pictogramTagTagId;
            while(downloadPictogramTag.next()) {
                pictogramTagPictogramId = downloadPictogramTag.getString("pictogram_id");
                pictogramTagTagId = downloadPictogramTag.getString("tag_id");
                Log.i(TAG, "downloadPictogramTag " + pictogramTagPictogramId + pictogramTagTagId + currentTime);
                db.insertPictogramTag(pictogramTagPictogramId, pictogramTagTagId, currentTime);
            }
            downloadPictogramTag.close();

            ResultSet downloadPictogramCategory = st.executeQuery("SELECT * FROM pictogram_category");
            String pictogramCategoryPictogramId, pictogramCategoryCategoryId;
            while(downloadPictogramCategory.next()) {
                pictogramCategoryPictogramId = downloadPictogramCategory.getString("pictogram_id");
                pictogramCategoryCategoryId = downloadPictogramCategory.getString("category_id");
                Log.i(TAG, "downloadPictogramCategory " + pictogramCategoryPictogramId + pictogramCategoryCategoryId + currentTime);
                db.insertPictogramCategory(pictogramCategoryPictogramId, pictogramCategoryCategoryId, currentTime);
            }
            downloadPictogramCategory.close();

            ResultSet downloadProfileCategory = st.executeQuery("SELECT * FROM profile_category");
            String profileCategoryProfileId, profileCategoryCategoryId;
            while(downloadProfileCategory.next()) {
                profileCategoryProfileId = downloadProfileCategory.getString("profile_id");
                profileCategoryCategoryId = downloadProfileCategory.getString("category_id");
                Log.i(TAG, "downloadProfileCategory " + profileCategoryProfileId + profileCategoryCategoryId + currentTime);
                db.insertProfileCategory(profileCategoryProfileId, profileCategoryCategoryId, currentTime);
            }
            downloadProfileCategory.close();

            ResultSet downloadGuardianOf = st.executeQuery("SELECT * FROM guardian_of");
            String guardianOfGuardianId, guardianOfChildId;
            while(downloadGuardianOf.next()) {
                guardianOfGuardianId = downloadGuardianOf.getString("guardian_id");
                guardianOfChildId = downloadGuardianOf.getString("child_id");
                Log.i(TAG, "downloadGuardianOf " + guardianOfGuardianId + guardianOfChildId + currentTime);
                db.insertGuardianOf(guardianOfGuardianId, guardianOfChildId, currentTime);
            }
            downloadGuardianOf.close();

            st.close();
            con.close();
            db.close();
        } catch (Exception e) {
        }

        return null;
    }
}
