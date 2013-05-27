CREATE VIEW `application_list` AS
    /* PROFILE */
    SELECT `user`.`id` AS `user_id`, `application`.`id`, `application`.`name`, (0) `author` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `profile_application` ON `profile`.`id`=`profile_application`.`profile_id`
        JOIN
        `application` ON `profile_application`.`application_id`=`application`.`id`
    UNION
    /* DEPARTMENT */
    SELECT `user`.`id` AS `user_id`, `application`.`id`, `application`.`name`, (0) `author` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `department` ON `profile`.`department_id`=`department`.`id`
        JOIN
        `department_application` ON `department`.`id`=`department_application`.`department_id`
        JOIN
        `application` ON `department_application`.`application_id`=`application`.`id`
    UNION
    /* CREATED applications */
    SELECT `user`.`id` AS `user_id`, `application`.`id`, `application`.`name`, (1) `author` FROM
        `user`
        JOIN
        `application` ON `user`.`id`=`application`.`author`;