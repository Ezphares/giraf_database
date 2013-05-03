USE `giraf`;
CREATE VIEW `giraf`.`profile_list` AS
    /* SELF */
    SELECT `user`.`id` AS `user_id`, `profile`.`id`, `profile`.`role`, `profile`.`name`, (1) `update`, (0) `delete` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
    UNION
    /* CHILDREN GUARDED BY SELF */
    SELECT `user`.`id` AS `user_id`, `profile`.`id`, `profile`.`role`, `profile`.`name`, (1) `update`, (0) `delete` FROM
        `user`
        JOIN
        `profile` AS `p_self` ON `user`.`id`=`p_self`.`user_id`
        JOIN
        `guardian_of` ON `p_self`.`id`=`guardian_of`.`guardian_id`
        JOIN
        `profile` ON `profile`.`id`=`guardian_of`.`child_id`
    UNION
    /* PARENTS OF SAID CHILDREN */
    SELECT `user`.`id` AS `user_id`, `profile`.`id`, `profile`.`role`, `profile`.`name`, (0) `update`, (0) `delete` FROM
        `user`
        JOIN
        `profile` AS `p_self` ON `user`.`id`=`p_self`.`user_id`
        JOIN
        `guardian_of` AS `g_guards` ON `p_self`.`id`=`g_guards`.`guardian_id`
        JOIN
        `profile` AS `p_guards` ON `p_guards`.`id`=`g_guards`.`child_id`
        JOIN
        `guardian_of` ON `p_guards`.`id` = `guardian_of`.`child_id`
        JOIN
        `profile` ON `profile`.`id`=`guardian_of`.`guardian_id`
        WHERE `profile`.`role`=1
    UNION
    /* ADMINISTERED PROFILES */
    SELECT `user`.`id` AS `user_id`, `profile`.`id`, `profile`.`role`, `profile`.`name`, (1) `update`, (1) `delete` FROM
        `user`
        JOIN
        `admin_of` ON `user`.`id`=`admin_of`.`user_id`
        JOIN
        `department` ON `admin_of`.`department_id`=`department`.`id`
        JOIN
        `profile` ON `department`.`id`=`profile`.`department_id`
        WHERE `profile`.`user_id`!=`user`.`id`;

CREATE VIEW `giraf`.`user_list` AS
    /* SELF */
    SELECT `user`.`id` AS `user_id`, `user`.`id`, `user`.`username`, (0) `delete` FROM
        `user`
    UNION
    /* ADMINISTRATED USERS */
    SELECT `u_self`.`id` AS `user_id`, `user`.`id`, `user`.`username`, (1) `delete` FROM
        `user` AS `u_self`
        JOIN
        `admin_of` ON `u_self`.`id`=`admin_of`.`user_id`
        JOIN
        `department` ON `admin_of`.`department_id`=`department`.`id`
        JOIN
        `profile` ON `department`.`id`=`profile`.`department_id`
        JOIN
        `user` ON `profile`.`user_id`=`user`.`id`
        WHERE `u_self`.`id`!=`user`.`id`;

CREATE VIEW `giraf`.`department_list` AS
    /* OWN */
    SELECT `user`.`id` AS `user_id`, `department`.`id`, `department`.`name`, (0) `update`, (0) `delete` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `department` ON `profile`.`department_id`=`department_id`
    UNION
    /* ADMIN */
    SELECT `user`.`id` AS `user_id`, `department`.`id`, `department`.`name`, (1) `update`, (0) `delete` FROM
        `user`
        JOIN
        `admin_of` ON `user`.`id`=`admin_of`.`user_id`
        JOIN
        `department` ON `admin_of`.`department_id`=`department`.`id`
    UNION
    /* ADMIN SUB */
    SELECT `user`.`id` AS `user_id`, `department`.`id`, `department`.`name`, (1) `update`, (1) `delete` FROM
        `user`
        JOIN
        `admin_of` ON `user`.`id`=`admin_of`.`user_id`
        JOIN
        `department` AS `p_admin` ON `admin_of`.`department_id`=`p_admin`.`id`
        JOIN
        `department` ON `p_admin`.`id`=`department`.`super_department_id`;

CREATE VIEW `pictogram_list` AS
    /* PROFILE */
    SELECT `user`.`id` AS `user_id`, `pictogram`.`id`, `pictogram`.`name`, (1) `direct` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `profile_pictogram` ON `profile`.`id`=`profile_pictogram`.`profile_id`
        JOIN
        `pictogram` ON `profile_pictogram`.`pictogram_id`=`pictogram`.`id`
    UNION
    /* DEPARTMENT */
    SELECT `user`.`id` AS `user_id`, `pictogram`.`id`, `pictogram`.`name`, (0) `direct` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `department` ON `profile`.`department_id`=`department`.`id`
        JOIN
        `department_pictogram` ON `department`.`id`=`department_pictogram`.`department_id`
        JOIN
        `pictogram` ON `department_pictogram`.`pictogram_id`=`pictogram`.`id`;
    
CREATE VIEW `application_list` AS
    /* PROFILE */
    SELECT `user`.`id` AS `user_id`, `application`.`id`, `application`.`name` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `profile_application` ON `profile`.`id`=`profile_application`.`profile_id`
        JOIN
        `application` ON `profile_application`.`application_id`=`application`.`id`
    UNION
    /* DEPARTMENT */
    SELECT `user`.`id` AS `user_id`, `application`.`id`, `application`.`name` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `department` ON `profile`.`department_id`=`department`.`id`
        JOIN
        `department_application` ON `department`.`id`=`department_application`.`department_id`
        JOIN
        `application` ON `department_application`.`application_id`=`application`.`id`;

CREATE VIEW `application_details` AS
    SELECT `user`.`id` AS `user_id`, `application`.*, `profile_application`.`settings`, (1) `direct` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `profile_application` ON `profile`.`id`=`profile_application`.`profile_id`
        JOIN
        `application` ON `profile_application`.`application_id`=`application`.`id`
    UNION
    /* DEPARTMENT */
    SELECT `user`.`id` AS `user_id`, `application`.*, ("") `settings`, (0) `direct` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `department` ON `profile`.`department_id`=`department`.`id`
        JOIN
        `department_application` ON `department`.`id`=`department_application`.`department_id`
        JOIN
        `application` ON `department_application`.`application_id`=`application`.`id`;

CREATE VIEW `pictogram_extras` AS
    SELECT `pictogram`.`id`, `category`.`name` AS `extra`, (1) `is_category` FROM
        `pictogram`
        JOIN
        `pictogram_category` ON `pictogram`.`id`=`pictogram_category`.`pictogram_id`
        JOIN
        `category` ON `pictogram_category`.`category_id`=`category`.`id`
    UNION
    SELECT `pictogram`.`id`, `tag`.`name` AS `extra`, (0) `is_category` FROM
        `pictogram`
        JOIN
        `pictogram_tag` ON `pictogram`.`id`=`pictogram_tag`.`pictogram_id`
        JOIN
        `tag` ON `pictogram_tag`.`tag_id`=`tag`.`id`;
