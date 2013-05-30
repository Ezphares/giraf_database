package dk.aau.cs.giraf.puddle;

import java.sql.BatchUpdateException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
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
		PreparedStatement user = null;
		PreparedStatement department = null;
		PreparedStatement profile = null;
		PreparedStatement pictogram = null;
		PreparedStatement tag = null;
		PreparedStatement category = null;
		PreparedStatement application = null;
		PreparedStatement adminOf  = null;
		PreparedStatement departmentPictogram = null;
		PreparedStatement profilePictogram = null;
		PreparedStatement departmentApplication = null;
		PreparedStatement profileApplication = null;
		PreparedStatement pictogramTag = null;
		PreparedStatement pictogramCategory = null;
		PreparedStatement profileCategory = null;
		PreparedStatement guardianOf = null;

		try {
			Log.i(TAG, "Trying connection");
			Connection con = null;

			try {
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
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (user != null) {
					try {
						user.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

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
					department = con.prepareStatement("INSERT INTO department (id, name, address, phone, email, super_department_id, author) VALUES(?, ?, ?, ?, ?, ?, ?) " +
							"ON DUPLICATE KEY UPDATE id=VALUES(id), name=VALUES(name), address=VALUES(address), phone=VALUES(phone), email=VALUES(email), super_department_id=VALUES(super_department_id), author=VALUES(author)");
					department.setString(1, departmentId);
					department.setString(2, departmentName);
					department.setString(3, departmentAddress);
					department.setString(4, departmentPhone);
					department.setString(5, departmentEmail);
					department.setString(6, departmentSuperDepartmentId);
					department.setString(7, departmentAuthor);
					department.addBatch();
					Log.i("uploadUpdatedDepartment", "getting " + departmentId + departmentName + departmentAddress + departmentPhone + departmentEmail + departmentSuperDepartmentId + departmentAuthor + profile);
					department.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (department != null) {
					try {
						department.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

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
					profile = con.prepareStatement("INSERT INTO profile (id, name, phone, picture, email, role, address, settings, user_id, department_id, author) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) " +
							"ON DUPLICATE KEY UPDATE id=VALUES(id), name=VALUES(name), phone=VALUES(phone), picture=VALUES(picture), email=VALUES(email), role=VALUES(role), address=VALUES(address), settings=VALUES(settings), user_id=VALUES(user_id), department_id=VALUES(department_id), author=VALUES(author)");
					profile.setString(1, profileId);
					profile.setString(2, profileName);
					profile.setString(3, profilePhone);
					profile.setString(4, profilePicture);
					profile.setString(5, profileEmail);
					profile.setString(6, profileRole);
					profile.setString(7, profileAddress);
					profile.setString(8, profileSettings);
					profile.setString(9, profileUserId);
					profile.setString(10, profileDepartmentId);
					profile.setString(11, profileAuthor);
					profile.addBatch();
					Log.i("uploadUpdatedProfile", "getting " + profileId + profileName + profilePhone + profilePicture + profileEmail + profileRole + profileAddress + profileSettings + profileUserId + profileDepartmentId + profileAuthor + profile);
					profile.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (profile != null) {
					try {
						profile.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

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
					pictogram = con.prepareStatement("INSERT INTO pictogram (id, name, public, image_data, sound_data, inline_text, author) VALUES(?, ?, ?, ?, ?, ?, ?) " +
							"ON DUPLICATE KEY UPDATE id=VALUES(id), name=VALUES(name), public=VALUES(public), image_data=VALUES(image_data), sound_data=VALUES(sound_data), inline_text=VALUES(inline_text), author=VALUES(author)");
					pictogram.setString(1, pictogramId);
					pictogram.setString(2, pictogramName);
					pictogram.setString(3, pictogramPublic);
					pictogram.setString(4, pictogramImageData);
					pictogram.setString(5, pictogramSoundData);
					pictogram.setString(6, pictogramInlineText);
					pictogram.setString(7, pictogramAuthor);
					pictogram.addBatch();
					Log.i(TAG, "uploadUpdatedPictogram " + pictogramId + pictogramName + pictogramPublic+ pictogramImageData + pictogramSoundData + pictogramInlineText + pictogramAuthor + pictogram);
					pictogram.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (pictogram != null) {
					try {
						pictogram.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedTag = db.getUpdated("tag");
				String tagId, tagName;
				while(uploadUpdatedTag.moveToNext()) {
					tagId = uploadUpdatedTag.getString(0);
					tagName = uploadUpdatedTag.getString(1);
					tag = con.prepareStatement("INSERT INTO tag (id, name) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE id=VALUES(id), name=VALUES(name)");
					tag.setString(1, tagId);
					tag.setString(2, tagName);
					tag.addBatch();
					Log.i(TAG, "uploadUpdatedTag " + tagId + tagName + tag);
					tag.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (tag != null) {
					try {
						tag.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedCategory = db.getUpdated("category");
				String categoryId, categoryName, categoryColour, categoryIcon, categorySuperCategoryId;
				while(uploadUpdatedCategory.moveToNext()) {
					categoryId = uploadUpdatedCategory.getString(0);
					categoryName = uploadUpdatedCategory.getString(1);
					categoryColour = uploadUpdatedCategory.getString(2);
					categoryIcon = uploadUpdatedCategory.getString(3);
					categorySuperCategoryId = uploadUpdatedCategory.getString(4);
					category = con.prepareStatement("INSERT INTO category (id, name, colour, icon, super_category_id) VALUES(?, ?, ?, ?, ?) " +
							"ON DUPLICATE KEY UPDATE id=VALUES(id), name=VALUES(name), colour=VALUES(colour), icon=VALUES(icon), super_category_id=VALUES(super_category_id)");
					category.setString(1, categoryId);
					category.setString(2, categoryName);
					category.setString(3, categoryColour);
					category.setString(4, categoryIcon);
					category.setString(5, categorySuperCategoryId);
					category.addBatch();
					Log.i(TAG, "uploadUpdatedCategory " + categoryId + categoryName + categoryColour+ categoryIcon + categorySuperCategoryId + category);
					category.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (category != null) {
					try {
						category.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

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
					application = con.prepareStatement("INSERT INTO application (id, name, version, icon, package, activity, description, author) VALUES(?, ?, ?, ?, ?, ?, ?, ?) " +
							"ON DUPLICATE KEY UPDATE id=VALUES(id), name=VALUES(name), version=VALUES(version), icon=VALUES(icon), package=VALUES(package), activity=VALUES(activity), description=VALUES(description), author=VALUES(author)");
					application.setString(1, applicationId);
					application.setString(2, applicationName);
					application.setString(3, applicationVersion);
					application.setString(4, applicationIcon);
					application.setString(5, applicationPackage);
					application.setString(6, applicationActivity);
					application.setString(7, applicationDescription);
					application.setString(8, applicationAuthor);
					application.addBatch();
					Log.i(TAG, "uploadUpdatedApplication " + applicationId + applicationName + applicationVersion+ applicationIcon + applicationPackage + applicationActivity + applicationDescription + applicationAuthor + application);
					application.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (application != null) {
					try {
						application.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedAdminOf = db.getUpdated("admin_of");
				String adminOfUserId, adminOfDepartmentId;
				while(uploadUpdatedAdminOf.moveToNext()) {
					adminOfUserId = uploadUpdatedAdminOf.getString(0);
					adminOfDepartmentId = uploadUpdatedAdminOf.getString(1);
					adminOf = con.prepareStatement("INSERT INTO admin_of (user_id, department_id) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE user_id=VALUES(user_id), department_id=VALUES(department_id)");
					adminOf.setString(1, adminOfUserId);
					adminOf.setString(2, adminOfDepartmentId);
					adminOf.addBatch();
					Log.i(TAG, "uploadUpdatedAdminOf " + adminOfUserId + adminOfDepartmentId + adminOf);
					adminOf.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (adminOf != null) {
					try {
						adminOf.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedDepartmentPictogram = db.getUpdated("department_pictogram");
				String departmentPictogtamDepartmentId, departmentPictogramPictogramId;
				while(uploadUpdatedDepartmentPictogram.moveToNext()) {
					departmentPictogtamDepartmentId = uploadUpdatedDepartmentPictogram.getString(0);
					departmentPictogramPictogramId = uploadUpdatedDepartmentPictogram.getString(1);
					departmentPictogram = con.prepareStatement("INSERT INTO department_pictogram (department_id, pictogram_id) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE department_id=VALUES(department_id), pictogram_id=VALUES(pictogram_id)");
					departmentPictogram.setString(1, departmentPictogtamDepartmentId);
					departmentPictogram.setString(2, departmentPictogramPictogramId);
					departmentPictogram.addBatch();
					Log.i(TAG, "uploadUpdatedDepartmentPictogram " + departmentPictogtamDepartmentId + departmentPictogramPictogramId + departmentPictogram);
					departmentPictogram.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (departmentPictogram != null) {
					try {
						departmentPictogram.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedProfilePictogram = db.getUpdated("profile_pictogram");
				String profilePictogramProfileId, profilePictogramPictogramId;
				while(uploadUpdatedProfilePictogram.moveToNext()) {
					profilePictogramProfileId = uploadUpdatedProfilePictogram.getString(0);
					profilePictogramPictogramId = uploadUpdatedProfilePictogram.getString(1);
					profilePictogram = con.prepareStatement("INSERT INTO profile_pictogram (profile_id, pictogram_id) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE profile_id=VALUES(profile_id), pictogram_id=VALUES(pictogram_id)");
					profilePictogram.setString(1, profilePictogramProfileId);
					profilePictogram.setString(2, profilePictogramPictogramId);
					profilePictogram.addBatch();
					Log.i(TAG, "uploadUpdatedProfilePictogram " + profilePictogramProfileId + profilePictogramPictogramId + profilePictogram);
					profilePictogram.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (profilePictogram != null) {
					try {
						profilePictogram.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedDepartmentApplication = db.getUpdated("department_application");
				String departmentApplicationDepartmentId, departmentApplicationApplicationId;
				while(uploadUpdatedDepartmentApplication.moveToNext()) {
					departmentApplicationDepartmentId = uploadUpdatedDepartmentApplication.getString(0);
					departmentApplicationApplicationId = uploadUpdatedDepartmentApplication.getString(1);
					departmentApplication = con.prepareStatement("INSERT INTO department_application (department_id, application_id) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE department_id=VALUES(department_id), application_id=VALUES(application_id)");
					departmentApplication.setString(1, departmentApplicationDepartmentId);
					departmentApplication.setString(2, departmentApplicationApplicationId);
					departmentApplication.addBatch();
					Log.i(TAG, "uploadUpdatedDepartmentApplication " + departmentApplicationDepartmentId + departmentApplicationApplicationId + departmentApplication);
					departmentApplication.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (departmentApplication != null) {
					try {
						departmentApplication.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}
			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedProfileApplication = db.getUpdated("profile_application");
				String profileApplicationProfileId, profileApplicationApplicationId, profileApplicationSettings;
				while(uploadUpdatedProfileApplication.moveToNext()) {
					profileApplicationProfileId = uploadUpdatedProfileApplication.getString(0);
					profileApplicationApplicationId = uploadUpdatedProfileApplication.getString(1);
					profileApplicationSettings = uploadUpdatedProfileApplication.getString(2);
					profileApplication = con.prepareStatement("INSERT INTO profile_application (profile_id, application_id, settings) VALUES(?, ?, ?) " +
							"ON DUPLICATE KEY UPDATE profile_id=VALUES(profile_id), application_id=VALUES(aplication_id), settings=VALUES(settings)");
					profileApplication.setString(1, profileApplicationProfileId);
					profileApplication.setString(2, profileApplicationApplicationId);
					profileApplication.setString(3, profileApplicationSettings);
					profileApplication.addBatch();
					Log.i(TAG, "uploadUpdatedProfileApplication " + profileApplicationProfileId + profileApplicationApplicationId + profileApplicationSettings + profileApplication);
					profileApplication.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (profileApplication != null) {
					try {
						profileApplication.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedPictogramTag = db.getUpdated("pictogram_tag");
				String pictogramTagPictogramId, pictogramTagTagId;
				while(uploadUpdatedPictogramTag.moveToNext()) {
					pictogramTagPictogramId = uploadUpdatedPictogramTag.getString(0);
					pictogramTagTagId = uploadUpdatedPictogramTag.getString(1);
					pictogramTag = con.prepareStatement("INSERT INTO pictogram_tag (pictogram_id, tag_id) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE pictogram_id=VALUES(pictogram_id), tag_id=VALUES(tag_id)");
					pictogramTag.setString(1, pictogramTagPictogramId);
					pictogramTag.setString(2, pictogramTagTagId);
					pictogramTag.addBatch();
					Log.i(TAG, "uploadUpdatedPictogramTag " + pictogramTagPictogramId + pictogramTagTagId + pictogramTag);
					pictogramTag.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (pictogramTag != null) {
					try {
						pictogramTag.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedPictogramCategory = db.getUpdated("pictogram_category");
				String pictogramCategoryPictogramId, pictogramCategoryCategoryId;
				while(uploadUpdatedPictogramCategory.moveToNext()) {
					pictogramCategoryPictogramId = uploadUpdatedPictogramCategory.getString(0);
					pictogramCategoryCategoryId = uploadUpdatedPictogramCategory.getString(1);
					pictogramCategory = con.prepareStatement("INSERT INTO pictogram_category (pictogram_id, category_id) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE pictogram_id=VALUES(pictogram_id), category_id=VALUES(category_id)");
					pictogramCategory.setString(1, pictogramCategoryPictogramId);
					pictogramCategory.setString(2, pictogramCategoryCategoryId);
					pictogramCategory.addBatch();
					Log.i(TAG, "uploadUpdatedPictogramCategory " + pictogramCategoryPictogramId + pictogramCategoryCategoryId + pictogramCategory);
					pictogramCategory.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (pictogramCategory != null) {
					try {
						pictogramCategory.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}
			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedProfileCategory = db.getUpdated("profile_category");
				String profileCategoryProfileId, profileCategoryCategoryId;
				while(uploadUpdatedProfileCategory.moveToNext()) {
					profileCategoryProfileId = uploadUpdatedProfileCategory.getString(0);
					profileCategoryCategoryId = uploadUpdatedProfileCategory.getString(1);
					profileCategory = con.prepareStatement("INSERT INTO profile_category (profile_id, category_id) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE profile_id=VALUES(profile_id), category_id=VALUES(category_id)");
					profileCategory.setString(1, profileCategoryProfileId);
					profileCategory.setString(2, profileCategoryCategoryId);
					profileCategory.addBatch();
					Log.i(TAG, "uploadUpdatedProfileCategory " + profileCategoryProfileId + profileCategoryCategoryId + profileCategory);
					profileCategory.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (profileCategory != null) {
					try {
						profileCategory.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			try {
				Class.forName("com.mysql.jdbc.Driver").newInstance();
				con = DriverManager.getConnection(MainActivity.DB_URL, MainActivity.DB_USER, MainActivity.DB_PASS);

				Cursor uploadUpdatedGuardianOf = db.getUpdated("guardian_of");
				String guardianOfGuardianId, guardianOfChildId;
				while(uploadUpdatedGuardianOf.moveToNext()) {
					guardianOfGuardianId = uploadUpdatedGuardianOf.getString(0);
					guardianOfChildId = uploadUpdatedGuardianOf.getString(1);
					guardianOf = con.prepareStatement("INSERT INTO guardian_of (guardian_id, child_id) VALUES(?, ?) " +
							"ON DUPLICATE KEY UPDATE guardian_id=VALUES(guardian_id), child_id=VALUES(child_id)");
					guardianOf.setString(1, guardianOfGuardianId);
					guardianOf.setString(2, guardianOfChildId);
					guardianOf.addBatch();
					Log.i(TAG, "uploadUpdatedGuardianOf " + guardianOfGuardianId + guardianOfChildId + guardianOf);
					guardianOf.executeBatch();
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (guardianOf != null) {
					try {
						guardianOf.close();
					} catch (SQLException e) {
					}
				}
				if (con != null) {
					try {
						con.close();
					} catch (SQLException e) {
					}
				}
			}

			db.close();
		} catch (Exception e) {
			Log.e(TAG, "Could not connect to server.");
		}

		return null;
	}

}