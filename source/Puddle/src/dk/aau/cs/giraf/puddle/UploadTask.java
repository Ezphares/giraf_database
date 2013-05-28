package dk.aau.cs.giraf.puddle;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.Statement;

import android.database.Cursor;
import android.os.AsyncTask;
import android.util.Log;

public class UploadTask extends AsyncTask{

    private static final String TAG = "UploadTask";
    Database db = null;

    @Override
    protected Object doInBackground(Object... params) {
        Log.i(TAG, "Hello from " + TAG);

        db = MainActivity.db;
        PreparedStatement pst = null;

        try {
            Log.i(TAG, "Trying connection");
            Class.forName("com.mysql.jdbc.Driver").newInstance();
            Connection con = DriverManager.getConnection(MainActivity.url, MainActivity.user, MainActivity.password);
            Statement st = con.createStatement();

            Log.i(TAG, "Uploading changes");
            Cursor uploadUpdatedUser = db.getUpdated("user");
            String userId, userUsername, userPassword, userCertificate;
            while(uploadUpdatedUser.moveToNext()) {
                userId = uploadUpdatedUser.getString(0);
                userUsername = uploadUpdatedUser.getString(1);
                userPassword = uploadUpdatedUser.getString(2);
                userCertificate = uploadUpdatedUser.getString(3);

                pst = con.prepareStatement("INSERT OR REPLACE INTO user (id, username, password, certificate) VALUES(?, ?, ?, ?)");
                pst.setString(1, userId);
                pst.setString(2, userUsername);
                pst.setString(3, userPassword);
                pst.setString(4, userCertificate);
                pst.addBatch();
                Log.i("uploadUpdatedUser", "getting " + userId + userUsername + userPassword + userCertificate + pst);
            }

            Cursor uploadUpdatedDepartment = db.getUpdated("department");
            String departmentId, departmentName, departmentAddress, departmentPhone, departmentEmail, departmentSuperDepartmentId, departmentAuthor;
            while(uploadUpdatedDepartment.moveToNext()) {
                departmentId = uploadUpdatedDepartment.getString(0);
                departmentName = uploadUpdatedDepartment.getString(1);
                departmentAddress = uploadUpdatedDepartment.getString(2);
                departmentPhone = uploadUpdatedDepartment.getString(3);
                departmentEmail = uploadUpdatedDepartment.getString(4);
                departmentSuperDepartmentId = uploadUpdatedDepartment.getString(5);
                departmentAuthor = uploadUpdatedDepartment.getString(6);
                pst = con.prepareStatement("INSERT OR REPLACE INTO department (id, name, address, phone, email, super_department_id, author) VALUES(?, ?, ?, ?, ?, ?, ?)");
                pst.setString(1, departmentId);
                pst.setString(2, departmentName);
                pst.setString(3, departmentAddress);
                pst.setString(4, departmentPhone);
                pst.setString(5, departmentEmail);
                pst.setString(6, departmentSuperDepartmentId);
                pst.setString(7, departmentAuthor);
                pst.addBatch();
                Log.i("uploadUpdatedDepartment", "getting " + departmentId + departmentName + departmentAddress + departmentPhone + departmentEmail + departmentSuperDepartmentId + departmentAuthor + pst);
            }

            Cursor uploadUpdatedProfile = db.getUpdated("profile");
            String profileId, profileName, profilePhone, profilePicture, profileEmail, profileRole, profileAddress, profileSettings, profileUserId, profileDepartmentId, profileAuthor;
            while(uploadUpdatedProfile.moveToNext()) {
                profileId = uploadUpdatedProfile.getString(0);
                profileName = uploadUpdatedProfile.getString(1);
                profilePhone = uploadUpdatedProfile.getString(2);
                profilePicture = uploadUpdatedProfile.getString(3);
                profileEmail = uploadUpdatedProfile.getString(4);
                profileRole = uploadUpdatedProfile.getString(5);
                profileAddress = uploadUpdatedProfile.getString(6);
                profileSettings = uploadUpdatedProfile.getString(7);
                profileUserId = uploadUpdatedProfile.getString(8);
                profileDepartmentId = uploadUpdatedProfile.getString(9);
                profileAuthor = uploadUpdatedProfile.getString(10);
                pst = con.prepareStatement("INSERT OR REPLACE INTO profile (id, name, phone, picture, email, role, address, settings, user_id, department_id, author) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                pst.setString(1, profileId);
                pst.setString(2, profileName);
                pst.setString(3, profilePhone);
                pst.setString(4, profilePicture);
                pst.setString(5, profileEmail);
                pst.setString(6, profileRole);
                pst.setString(7, profileAddress);
                pst.setString(8, profileSettings);
                pst.setString(9, profileUserId);
                pst.setString(10, profileDepartmentId);
                pst.setString(11, profileAuthor);
                pst.addBatch();
                Log.i("uploadUpdatedProfile", "getting " + profileId + profileName + profilePhone + profilePicture + profileEmail + profileRole + profileAddress + profileSettings + profileUserId + profileDepartmentId + profileAuthor + pst);
            }

            Cursor uploadUpdatedPictogram = db.getUpdated("pictogram");
            String pictogramId, pictogramName, pictogramPublic, pictogramImageData, pictogramSoundData, pictogramInlineText, pictogramAuthor;
            while(uploadUpdatedPictogram.moveToNext()) {
                pictogramId = uploadUpdatedPictogram.getString(0);
                pictogramName = uploadUpdatedPictogram.getString(1);
                pictogramPublic = uploadUpdatedPictogram.getString(2);
                pictogramImageData = uploadUpdatedPictogram.getString(3);
                pictogramSoundData = uploadUpdatedPictogram.getString(4);
                pictogramInlineText = uploadUpdatedPictogram.getString(5);
                pictogramAuthor = uploadUpdatedPictogram.getString(6);
                pst = con.prepareStatement("INSERT OR REPLACE INTO pictogram (id, name, public, image_data, sound_data, inline_text, author) VALUES(?, ?, ?, ?, ?, ?, ?)");
                pst.setString(1, pictogramId);
                pst.setString(2, pictogramName);
                pst.setString(3, pictogramPublic);
                pst.setString(4, pictogramImageData);
                pst.setString(5, pictogramSoundData);
                pst.setString(6, pictogramInlineText);
                pst.setString(7, pictogramAuthor);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedPictogram " + pictogramId + pictogramName + pictogramPublic+ pictogramImageData + pictogramSoundData + pictogramInlineText + pictogramAuthor + pst);
            }

            Cursor uploadUpdatedTag = db.getUpdated("tag");
            String tagId, tagName;
            while(uploadUpdatedTag.moveToNext()) {
                tagId = uploadUpdatedTag.getString(0);
                tagName = uploadUpdatedTag.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO tag (id, name) VALUES(?, ?)");
                pst.setString(1, tagId);
                pst.setString(2, tagName);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedTag " + tagId + tagName + pst);
            }

            Cursor uploadUpdatedCategory = db.getUpdated("category");
            String categoryId, categoryName, categoryColour, categoryIcon, categorySuperCategoryId;
            while(uploadUpdatedCategory.moveToNext()) {
                categoryId = uploadUpdatedCategory.getString(0);
                categoryName = uploadUpdatedCategory.getString(1);
                categoryColour = uploadUpdatedCategory.getString(2);
                categoryIcon = uploadUpdatedCategory.getString(3);
                categorySuperCategoryId = uploadUpdatedCategory.getString(4);
                pst = con.prepareStatement("INSERT OR REPLACE INTO category (id, name, colour, icon, super_category_id) VALUES(?, ?, ?, ?, ?)");
                pst.setString(1, categoryId);
                pst.setString(2, categoryName);
                pst.setString(3, categoryColour);
                pst.setString(4, categoryIcon);
                pst.setString(5, categorySuperCategoryId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedCategory " + categoryId + categoryName + categoryColour+ categoryIcon + categorySuperCategoryId + pst);
            }

            Cursor uploadUpdatedApplication = db.getUpdated("application");
            String applicationId, applicationName, applicationVersion, applicationIcon, applicationPackage, applicationActivity, applicationDescription, applicationAuthor;
            while(uploadUpdatedApplication.moveToNext()) {
                applicationId = uploadUpdatedApplication.getString(0);
                applicationName = uploadUpdatedApplication.getString(1);
                applicationVersion = uploadUpdatedApplication.getString(2);
                applicationIcon = uploadUpdatedApplication.getString(3);
                applicationPackage = uploadUpdatedApplication.getString(4);
                applicationActivity = uploadUpdatedApplication.getString(5);
                applicationDescription = uploadUpdatedApplication.getString(6);
                applicationAuthor = uploadUpdatedApplication.getString(7);
                pst = con.prepareStatement("INSERT OR REPLACE INTO application (id, name, version, icon, package, activity, description, author) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
                pst.setString(1, applicationId);
                pst.setString(2, applicationName);
                pst.setString(3, applicationVersion);
                pst.setString(4, applicationIcon);
                pst.setString(5, applicationPackage);
                pst.setString(6, applicationActivity);
                pst.setString(7, applicationDescription);
                pst.setString(8, applicationAuthor);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedApplication " + applicationId + applicationName + applicationVersion+ applicationIcon + applicationPackage + applicationActivity + applicationDescription + applicationAuthor + pst);
            }

            Cursor uploadUpdatedAdminOf = db.getUpdated("admin_of");
            String adminOfUserId, adminOfDepartmentId;
            while(uploadUpdatedAdminOf.moveToNext()) {
                adminOfUserId = uploadUpdatedAdminOf.getString(0);
                adminOfDepartmentId = uploadUpdatedAdminOf.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO admin_of (user_id, department_id) VALUES(?, ?)");
                pst.setString(1, adminOfUserId);
                pst.setString(2, adminOfDepartmentId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedAdminOf " + adminOfUserId + adminOfDepartmentId + pst);
            }

            Cursor uploadUpdatedDepartmentPictogram = db.getUpdated("department_pictogram");
            String departmentPictogtamDepartmentId, departmentPictogramPictogramId;
            while(uploadUpdatedDepartmentPictogram.moveToNext()) {
                departmentPictogtamDepartmentId = uploadUpdatedDepartmentPictogram.getString(0);
                departmentPictogramPictogramId = uploadUpdatedDepartmentPictogram.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO department_pictogram (department_id, pictogram_id) VALUES(?, ?)");
                pst.setString(1, departmentPictogtamDepartmentId);
                pst.setString(2, departmentPictogramPictogramId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedDepartmentPictogram " + departmentPictogtamDepartmentId + departmentPictogramPictogramId + pst);
            }

            Cursor uploadUpdatedProfilePictogram = db.getUpdated("profile_pictogram");
            String profilePictogramProfileId, profilePictogramPictogramId;
            while(uploadUpdatedProfilePictogram.moveToNext()) {
                profilePictogramProfileId = uploadUpdatedProfilePictogram.getString(0);
                profilePictogramPictogramId = uploadUpdatedProfilePictogram.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO profile_pictogram (profile_id, pictogram_id) VALUES(?, ?)");
                pst.setString(1, profilePictogramProfileId);
                pst.setString(2, profilePictogramPictogramId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedProfilePictogram " + profilePictogramProfileId + profilePictogramPictogramId + pst);
            }

            Cursor uploadUpdatedDepartmentApplication = db.getUpdated("department_application");
            String departmentApplicationDepartmentId, departmentApplicationApplicationId;
            while(uploadUpdatedDepartmentApplication.moveToNext()) {
                departmentApplicationDepartmentId = uploadUpdatedDepartmentApplication.getString(0);
                departmentApplicationApplicationId = uploadUpdatedDepartmentApplication.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO department_application (department_id, application_id) VALUES(?, ?)");
                pst.setString(1, departmentApplicationDepartmentId);
                pst.setString(2, departmentApplicationApplicationId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedDepartmentApplication " + departmentApplicationDepartmentId + departmentApplicationApplicationId + pst);
            }

            Cursor uploadUpdatedProfileApplication = db.getUpdated("profile_application");
            String profileApplicationProfileId, profileApplicationApplicationId, profileApplicationSettings;
            while(uploadUpdatedProfileApplication.moveToNext()) {
                profileApplicationProfileId = uploadUpdatedProfileApplication.getString(0);
                profileApplicationApplicationId = uploadUpdatedProfileApplication.getString(1);
                profileApplicationSettings = uploadUpdatedProfileApplication.getString(2);
                pst = con.prepareStatement("INSERT OR REPLACE INTO profile_application (profile_id, application_id, settings) VALUES(?, ?, ?)");
                pst.setString(1, profileApplicationProfileId);
                pst.setString(2, profileApplicationApplicationId);
                pst.setString(3, profileApplicationSettings);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedProfileApplication " + profileApplicationProfileId + profileApplicationApplicationId + profileApplicationSettings + pst);
            }

            Cursor uploadUpdatedPictogramTag = db.getUpdated("pictogram_tag");
            String pictogramTagPictogramId, pictogramTagTagId;
            while(uploadUpdatedPictogramTag.moveToNext()) {
                pictogramTagPictogramId = uploadUpdatedPictogramTag.getString(0);
                pictogramTagTagId = uploadUpdatedPictogramTag.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO pictogram_tag (pictogram_id, tag_id) VALUES(?, ?)");
                pst.setString(1, pictogramTagPictogramId);
                pst.setString(2, pictogramTagTagId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedPictogramTag " + pictogramTagPictogramId + pictogramTagTagId + pst);
            }

            Cursor uploadUpdatedPictogramCategory = db.getUpdated("pictogram_category");
            String pictogramCategoryPictogramId, pictogramCategoryCategoryId;
            while(uploadUpdatedPictogramCategory.moveToNext()) {
                pictogramCategoryPictogramId = uploadUpdatedPictogramCategory.getString(0);
                pictogramCategoryCategoryId = uploadUpdatedPictogramCategory.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO pictogram_category (pictogram_id, category_id) VALUES(?, ?)");
                pst.setString(1, pictogramCategoryPictogramId);
                pst.setString(2, pictogramCategoryCategoryId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedPictogramCategory " + pictogramCategoryPictogramId + pictogramCategoryCategoryId + pst);
            }

            Cursor uploadUpdatedProfileCategory = db.getUpdated("profile_category");
            String profileCategoryProfileId, profileCategoryCategoryId;
            while(uploadUpdatedProfileCategory.moveToNext()) {
                profileCategoryProfileId = uploadUpdatedProfileCategory.getString(0);
                profileCategoryCategoryId = uploadUpdatedProfileCategory.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO profile_category (profile_id, category_id) VALUES(?, ?)");
                pst.setString(1, profileCategoryProfileId);
                pst.setString(2, profileCategoryCategoryId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedProfileCategory " + profileCategoryProfileId + profileCategoryCategoryId + pst);
            }

            Cursor uploadUpdatedGuardianOf = db.getUpdated("guardian_of");
            String guardianOfGuardianId, guardianOfChildId;
            while(uploadUpdatedGuardianOf.moveToNext()) {
                guardianOfGuardianId = uploadUpdatedGuardianOf.getString(0);
                guardianOfChildId = uploadUpdatedGuardianOf.getString(1);
                pst = con.prepareStatement("INSERT OR REPLACE INTO guardian_of (guardian_id, child_id) VALUES(?, ?)");
                pst.setString(1, guardianOfGuardianId);
                pst.setString(2, guardianOfChildId);
                pst.addBatch();
                Log.i(TAG, "uploadUpdatedGuardianOf " + guardianOfGuardianId + guardianOfChildId + pst);
            }

            con.setAutoCommit(false);
            pst.executeBatch();
            con.setAutoCommit(true);

            db.close();
            pst.close();
            st.close();
            con.close();
        } catch (Exception e) {
        }

        return null;
    }

}
