SET @user=%d;
SELECT DISTINCT `department`.`id`, `department`.`name`
	FROM 
		`department`
		JOIN
		`profile`
		ON `department`.`id`=`profile`.`department_id`
	WHERE `profile`.`user_id`=@user
UNION
SELECT DISTINCT`department`.`id`, `department`.`name`
	FROM
		`department`
		JOIN
		`admin_of`
		ON `department`.`id`=`admin_of`.`department_id`
	WHERE `admin_of`.`user_id`=@user
UNION
SELECT DISTINCT `department`.`id`, `department`.`name`
	FROM
		(`department` AS `d_top`
		JOIN
		`admin_of`
		ON `department`.`id`=`admin_of`.`department_id`)
		JOIN
		`department`
		ON `department`.`super_department_id`=`d_top`.`id`
	WHERE `admin_of`.`user_id`=@user;
