SET @user=%d;
SELECT DISTINCT `profile_pictogram`.`pictogram_id` AS `id`
	FROM
		`profile`
		JOIN
		`profile_pictogram`
		ON `profile`.`id`=`profile_pictogram`.`profile_id`
	WHERE `profile`.`user_id`=@user
UNION
SELECT DISTINCT `department_pictogram`.`pictogram_id` AS `id`
	FROM
		(`profile`
		JOIN
		`department`
		ON `profile`.`department_id`=`department`.`id`)
		JOIN
		`department_pictogram`
		ON `department`.`id`=`department_pictogram`.`department_id`
	WHERE `profile`.`user_id`=@user;

