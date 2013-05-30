CREATE DATABASE IF NOT EXISTS `giraf`
	DEFAULT CHARACTER SET latin1;
	
USE `giraf`;

CREATE TABLE `user` (
	`id`			INT(11)			NOT NULL	AUTO_INCREMENT,
	`username`		VARCHAR(64)		NOT NULL	UNIQUE,
	`password`		VARCHAR(256)	NULL,
	`certificate`	VARCHAR(512)	NULL	UNIQUE,
	PRIMARY KEY (`id`)
);

CREATE TABLE `department` (
	`id`					INT(11)			NOT NULL	AUTO_INCREMENT,
	`name`					VARCHAR(64)		NOT NULL,
	`address`				VARCHAR(256)	NOT NULL,
	`phone`					VARCHAR(12)			NOT NULL,	
	`email`					VARCHAR(64)		NOT NULL,
	`super_department_id`	INT(11)		NULL,
	`author`				INT(11)		NULL,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`super_department_id`)
		REFERENCES `department` (`id`)
		ON DELETE SET NULL,
	FOREIGN KEY (`author`)
		REFERENCES `user` (`id`)
		ON DELETE SET NULL
);

CREATE TABLE `profile` (
	`id`				INT(11)			NOT NULL	AUTO_INCREMENT,
	`name`				VARCHAR(64)		NOT NULL,
	`phone`				VARCHAR(12)		NULL,
	`picture`			BLOB			NULL,
	`email`				VARCHAR(64)		NULL,
	`role`				SMALLINT		NOT NULL,
	`address`			VARCHAR(256)	NOT NULL,
	`settings`			BLOB			NULL,
	`user_id`			INT(11)			NULL		UNIQUE,
	`department_id`		INT(11)			NOT NULL,
    `author`            INT(11)         NULL,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`user_id`)
		REFERENCES `user` (`id`)
		ON DELETE SET NULL,
	FOREIGN KEY (`department_id`)
		REFERENCES `department` (`id`)
		ON DELETE CASCADE,
    FOREIGN KEY (`author`)
        REFERENCES `profile` (`id`)
        ON DELETE SET NULL
);

CREATE TABLE `pictogram` (
	`id`			INT(11)			NOT NULL	AUTO_INCREMENT,
	`name`			VARCHAR(64)		NOT NULL,
	`public`		TINYINT			NOT NULL,
	`image_data`	BLOB			NULL,
	`sound_data`	BLOB			NULL,
	`inline_text`	VARCHAR(64)		NULL,
	`author`		INT(11)		NULL,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`author`)
		REFERENCES `user` (`id`)
		ON DELETE SET NULL
);

CREATE TABLE `tag` (
	`id`	INT(11)			NOT NULL 	AUTO_INCREMENT,
	`name`	VARCHAR(64)		NOT NULL,
	PRIMARY KEY (`id`)
);

CREATE TABLE `category` (
	`id`					INT(11)			NOT NULL	AUTO_INCREMENT,
	`name`					VARCHAR(64)		NOT NULL,
	`colour`				VARCHAR(11)		NOT NULL,
	`icon`					BLOB			NULL,
	`super_category_id`		INT(11)			NULL,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`super_category_id`) 
		REFERENCES `category` (`id`)
		ON DELETE SET NULL
);

CREATE TABLE `application` (
	`id`			INT(11)			NOT NULL	AUTO_INCREMENT,
	`name`			VARCHAR(64) 	NOT NULL,
	`version`		VARCHAR(32)		NOT NULL,
	`icon`			BLOB			NOT NULL,
	`package`		VARCHAR(256)	NOT NULL,
	`activity`		VARCHAR(64)		NOT NULL,
	`description`	VARCHAR(1024)	NOT NULL,
	`author`		INT(11)			NULL,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`author`)
		REFERENCES `user` (`id`)
		ON DELETE SET NULL
);

CREATE TABLE `admin_of`(
	`user_id`			INT(11)		NOT NULL,	
	`department_id`		INT(11)		NOT NULL,
    PRIMARY KEY (`user_id`, `department_id`),
	FOREIGN KEY (`user_id`)
		REFERENCES `user` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`department_id`)
		REFERENCES `department` (`id`)
		ON DELETE CASCADE
);

CREATE TABLE `department_pictogram` (
	`department_id`		INT(11)		NOT NULL,	
	`pictogram_id`		INT(11)		NOT NULL,
    PRIMARY KEY (`department_id`, `pictogram_id`),
	FOREIGN KEY (`department_id`)
		REFERENCES `department` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`pictogram_id`)
		REFERENCES `pictogram` (`id`)
		ON DELETE CASCADE
);

CREATE TABLE `profile_pictogram` (
	`profile_id`		INT(11)		NOT NULL,	
	`pictogram_id`		INT(11)		NOT NULL,
    PRIMARY KEY (`profile_id`, `pictogram_id`),
	FOREIGN KEY (`profile_id`)
		REFERENCES `profile` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`pictogram_id`)
		REFERENCES `pictogram` (`id`)
		ON DELETE CASCADE
);

CREATE TABLE `department_application` (
	`department_id`		INT(11)		NOT NULL,
	`application_id`	INT(11)		NOT NULL,
    PRIMARY KEY (`department_id`, `application_id`),	
	FOREIGN KEY (`department_id`)
		REFERENCES `department` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`application_id`)
		REFERENCES `application` (`id`)
		ON DELETE CASCADE
);

CREATE TABLE `profile_application`(
	`profile_id`		INT(11)		NOT NULL,
	`application_id`	INT(11)		NOT NULL,	
	`settings`			BLOB		NULL,
    PRIMARY KEY (`profile_id`, `application_id`),
	FOREIGN KEY (`profile_id`)
		REFERENCES `profile` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`application_id`)
		REFERENCES `application` (`id`)
		ON DELETE CASCADE
);

CREATE TABLE `pictogram_tag` (
	`pictogram_id`		INT(11)		NOT NULL,
	`tag_id`			INT(11)		NOT NULL,	
    PRIMARY KEY (`tag_id`, `pictogram_id`),
	FOREIGN KEY (`pictogram_id`)
		REFERENCES `pictogram` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`tag_id`)
		REFERENCES `tag` (`id`)
		ON DELETE CASCADE
);

CREATE TABLE `pictogram_category` (
	`pictogram_id`		INT(11)		NOT NULL,
	`category_id`	INT(11)		NOT NULL,	
    PRIMARY KEY (`category_id`, `pictogram_id`),
	FOREIGN KEY (`pictogram_id`)
		REFERENCES `pictogram` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`category_id`)
		REFERENCES `category` (`id`)
		ON DELETE CASCADE
);

CREATE TABLE `profile_category` (
	`profile_id`		INT(11)		NOT NULL,
	`category_id`		INT(11)		NOT NULL,
    PRIMARY KEY (`profile_id`, `category_id`),
	FOREIGN KEY (`profile_id`)
		REFERENCES `profile` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`category_id`)
		REFERENCES `category` (`id`)
		ON DELETE CASCADE
);

CREATE TABLE `guardian_of` (
	`guardian_id`	INT(11)		NOT NULL,
	`child_id`		INT(11)		NOT NULL,
    PRIMARY KEY (`guardian_id`, `child_id`),
	FOREIGN KEY (`guardian_id`)
		REFERENCES `profile` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`child_id`)
		REFERENCES `profile` (`id`)
		ON DELETE CASCADE
);

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
        WHERE (`profile`.`user_id` IS NULL) OR (`user`.`id` != `profile`.`user_id`)
    UNION
    /* CREATED PROFILES */
    SELECT `user`.`id` AS `user_id`, `profile`.`id`, `profile`.`role`, `profile`.`name`, (1) `update`, (1) `delete` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`= `profile`.`author`;


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
        `department` ON `p_admin`.`id`=`department`.`super_department_id`
    UNION
    /* CREATED DEPARTMENTS */
    SELECT `user`.`id` AS `user_id`, `department`.`id`, `department`.`name`, (1) `update`, (1) `delete` FROM
        `user`
        JOIN
        `department` ON `user`.`id`=`department`.`author`;
        
CREATE VIEW `pictogram_list` AS
    /* PROFILE */
    SELECT `user`.`id` AS `user_id`, `pictogram`.`id`, `pictogram`.`name`, (1) `direct`, (0) `author` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `profile_pictogram` ON `profile`.`id`=`profile_pictogram`.`profile_id`
        JOIN
        `pictogram` ON `profile_pictogram`.`pictogram_id`=`pictogram`.`id`
    UNION
    /* DEPARTMENT */
    SELECT `user`.`id` AS `user_id`, `pictogram`.`id`, `pictogram`.`name`, (0) `direct`, (0) `author` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `department` ON `profile`.`department_id`=`department`.`id`
        JOIN
        `department_pictogram` ON `department`.`id`=`department_pictogram`.`department_id`
        JOIN
        `pictogram` ON `department_pictogram`.`pictogram_id`=`pictogram`.`id`
    UNION
    /* CREATED PICTOGRAMS */
    SELECT `user`.`id` AS `user_id`, `pictogram`.`id`, `pictogram`.`name`, (0) `direct`, (1) `author` FROM
        `user`
        JOIN
        `pictogram` ON `user`.`id`=`pictogram`.`author`;
    
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

CREATE VIEW `category_list` AS
    SELECT `user`.`id` AS `user_id`, `category`.`id`, `category`.`name`, `category`.`super_category_id` FROM
        `user`
        JOIN
        `profile` ON `user`.`id`=`profile`.`user_id`
        JOIN
        `profile_category` ON `profile`.`id` = `profile_category`.`profile_id`
        JOIN
        `category` ON `profile_category`.`category_id` = `category`.`id`;
