USE `giraf`;

INSERT INTO `department` (`name`, `address`, `phone`, `email`)
        VALUES ("Cassiopeia", "Selma Lagerløfs Vej 300", 12345678, "test@test.com");

INSERT INTO `user` (`username`, `password`, `certificate`)
    VALUES ('john', '123456', '123456');

INSERT INTO `profile` (`name`, `role`, `address`, `department_id`)
        VALUES ("John Smith", 1, "Herp Derpson 2", 1),
               ("Lone Jensen", 0, "Herpie Herpson 123", 1),
               ("Derpie Derpie", 1, "Herp Derpson 2", 1),
               ("I haz an autism", 2, "Herp Derpson 2", 1),
               ("ANDERS", 2, "Jens Bangs Vej 23", 1);

UPDATE `profile`
    SET `user_id`=1
    WHERE `id`=1;

INSERT INTO `guardian_of` (`guardian_id`, `child_id`)
        VALUES (1, 4),
               (2, 4),
               (3, 4),
               (2, 5);

INSERT INTO `pictogram` (`name`, `public`) VALUES ('bil', false), ('hus', true);

INSERT INTO `profile_pictogram` (`profile_id`, `pictogram_id`) VALUES (1, 1), (1, 2);

INSERT INTO `application` (`name`, `version`, `icon`, `package`, `activity`, `description`)
        VALUES ('Tog', '1.0', 'Herp', 'deeerp', 'ANDERS', 'hurr durr');

INSERT INTO `profile_application` (`profile_id`, `application_id`) VALUES (1, 1);

INSERT INTO `admin_of` (`user_id`, `department_id`) VALUES (1, 1);