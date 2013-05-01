CREATE DATABASE IF NOT EXISTS `giraf`
	DEFAULT CHARACTER SET latin1;
	
USE `giraf`;

CREATE TABLE `user` (
	`id`			INT(11)			NOT NULL	AUTO_INCREMENT,
	`username`		VARCHAR(64)		NOT NULL	UNIQUE,
	`password`		VARCHAR(256)	NOT NULL,
	`certificate`	VARCHAR(512)	NOT NULL	UNIQUE,
	PRIMARY KEY (`id`)
);

CREATE TABLE `department` (
	`id`					INT(11)			NOT NULL	AUTO_INCREMENT,
	`name`					VARCHAR(64)		NOT NULL,
	`address`				VARCHAR(256)	NOT NULL,
	`phone`					INT(11)			NOT NULL,	
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
	`phone`				INT(11)			NULL,
	`picture`			BLOB			NULL,
	`email`				VARCHAR(64)		NULL,
	`role`				SMALLINT		NOT NULL,
	`address`			VARCHAR(256)	NOT NULL,
	`settings`			BLOB			NULL,
	`user_id`			INT(11)			NULL		UNIQUE,
	`department_id`		INT(11)			NOT NULL,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`user_id`)
		REFERENCES `user` (`id`)
		ON DELETE SET NULL,
	FOREIGN KEY (`department_id`)
		REFERENCES `department` (`id`)
		ON DELETE CASCADE
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
	FOREIGN KEY (`guardian_id`)
		REFERENCES `profile` (`id`)
		ON DELETE CASCADE,
	FOREIGN KEY (`child_id`)
		REFERENCES `profile` (`id`)
		ON DELETE CASCADE
);











