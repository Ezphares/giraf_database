SELECT `id`, `name`, `role`
    FROM `profile`
    WHERE `user_id`=%d
UNION
SELECT `children`.`id`, `children`.`name`, `children`.`role` 
    FROM ((`guardian_of`
    JOIN `profile` AS `p_self`
    ON `guardian_of`.`guardian_id`=`p_self`.`id`)
    JOIN `profile` AS `children`
    ON `children`.`id`=`guardian_of`.`child_id`) 
    WHERE`p_self`.`user_id`=%d
UNION
SELECT DISTINCT `parents`.`id`, `parents`.`name`, `parents`.`role` 
    FROM ((((`guardian_of` 
    JOIN `profile` AS `p_self`
    ON `guardian_of`.`guardian_id`=`p_self`.`id`)
    JOIN `profile` AS `children`
    ON `children`.`id`=`guardian_of`.`child_id`)
    JOIN `guardian_of` AS `g_1`
    ON `children`.`id`=`g_1`.`child_id`)
    JOIN `profile` AS `parents`
    ON `parents`.`id`=`g_1`.`guardian_id`)
    WHERE`p_self`.`user_id`=%d AND `parents`.`role`=1;
