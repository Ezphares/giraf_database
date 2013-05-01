SET @user=%d;
SELECT DISTINCT `user`.`id`, `user`.`username`
	FROM `user`
	WHERE `user`.`id`=@user
UNION
SELECT DISTINCT `user`.`id`, `user`.`username`
	FROM
		((`admin_of`
		JOIN
		`department`
		ON `admin_of`.`department_id`=`department`.`id`)
		JOIN
		`profile`
		ON `department`.`id`=`profile`.`department`.`id`)
		JOIN
		`user`
		ON `profile`.`user_id`=`user`.id`
	WHERE `admin_of`.`user_id`=@user;

