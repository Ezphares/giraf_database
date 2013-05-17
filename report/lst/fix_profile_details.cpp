void fix_profile_details(Json::Value &o)
{
	fix_rename(o, "department_id", "department");
	fix_remove(o, "user_id");
	fix_type(o, "id", V_INT);
	fix_type(o, "role", V_INT);
	fix_type(o, "department", V_INT);
}