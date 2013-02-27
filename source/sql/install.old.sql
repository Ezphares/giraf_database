# Ensure database exists
CREATE DATABASE IF NOT EXISTS `savannah`
    DEFAULT CHARACTER SET latin1;

USE `savannah`;

# Create new tables
CREATE TABLE `AuthUsers` (
    `certificate`   VARCHAR(512)    NOT NULL    UNIQUE,
    `idUser`        INT(11)         NOT NULL    AUTO_INCREMENT,
    `aRole`         INT(11)         NOT NULL,
    `username`      VARCHAR(45)     NOT NULL    UNIQUE,
    `password`      VARCHAR(45)     NOT NULL,
    PRIMARY KEY (`certificate`),
    UNIQUE INDEX `idUser_UNIQUE` (`idUser` ASC)
);

CREATE TABLE `Profile` (
    `idProfile`     INT(11)         NOT NULL    AUTO_INCREMENT,
    `firstname`     VARCHAR(45)     NOT NULL,
    `surname`       VARCHAR(45)     NOT NULL,
    `middlename`    VARCHAR(45)     NULL,
    `pRole`         INT(11)         NOT NULL,
    `phone`         BIGINT(20)      NOT NULL,
    `picture`       VARCHAR(45)     NULL,
    `settings`      BLOB            NULL,
    PRIMARY KEY (`idProfile`)
);

CREATE TABLE `HasGuardian` (
    `idGuardian`    INT(11)         NOT NULL,
    `idChild`       INT(11)         NOT NULL,
    FOREIGN KEY (`idGuardian`)
        REFERENCES `Profile` (`idProfile`)
        ON DELETE CASCADE,
    FOREIGN KEY (`idChild`)
        REFERENCES `Profile` (`idProfile`) 
        ON DELETE CASCADE
);

CREATE TABLE `Apps` (
    `idApp`         INT(11)         NOT NULL    AUTO_INCREMENT,
    `name`          VARCHAR(45)     NOT NULL,
    `version`       VARCHAR(45)     NOT NULL,
    `icon`          VARCHAR(45)     NOT NULL,
    `package`       VARCHAR(45)     NOT NULL,
    `activity`      VARCHAR(45)     NOT NULL,
    PRIMARY KEY (`idApp`)
);

CREATE TABLE `ListOfApps` (
    `idApp`         INT(11)         NOT NULL,
    `idProfile`     INT(11)         NOT NULL,
    `settings`      BLOB            NULL,
    `stats`         BLOB            NULL,
    FOREIGN KEY (`idApp`)
        REFERENCES `Apps` (`idApp`)
        ON DELETE CASCADE,
    FOREIGN KEY (`idProfile`)
        REFERENCES `Profile` (`idProfile`)
        ON DELETE CASCADE
);

CREATE TABLE `Media` (
    `idMedia`       INT(11)         NOT NULL    AUTO_INCREMENT,
    `mPath`         VARCHAR(45)     NOT NULL,
    `name`          VARCHAR(45)     NOT NULL,
    `mPublic`       TINYINT(4)      NOT NULL,
    `mType`         VARCHAR(45)     NOT NULL,
    `ownerID`       INT(11)         NOT NULL,
    PRIMARY KEY (`idMedia`)
);

CREATE TABLE `Tags` (
    `idTags`        INT(11)         NOT NULL    AUTO_INCREMENT,
    `caption`       VARCHAR(45)     NOT NULL,
    PRIMARY KEY (`idTags`)
); 

CREATE TABLE `HasTag` (
    `idMedia`       INT(11)         NOT NULL,
    `idTag`         INT(11)         NOT NULL,
    FOREIGN KEY (`idMedia`)
        REFERENCES `Media` (`idMedia`)
        ON DELETE CASCADE,
    FOREIGN KEY (`idTag`)
        REFERENCES `Tags` (`idTags`)
        ON DELETE CASCADE
);

CREATE TABLE `MediaProfileAccess` (
    `idProfile`     INT(11)         NOT NULL,
    `idMedia`       INT(11)         NOT NULL,
    FOREIGN KEY (`idProfile`)
        REFERENCES `Profile` (`idProfile`)
        ON DELETE CASCADE,
    FOREIGN KEY (`idMedia`)
        REFERENCES `Media` (`idMedia`)
        ON DELETE CASCADE
);

CREATE TABLE `HasLink` (
    `idMedia`       INT(11)         NOT NULL,
    `idSubMedia`    INT(11)         NOT NULL,
    FOREIGN KEY (`idMedia`)
        REFERENCES `Media` (`idMedia`)
        ON DELETE CASCADE,
    FOREIGN KEY (`idSubMedia`)
        REFERENCES `Media` (`idMedia`)
        ON DELETE CASCADE
);

CREATE TABLE `Department` (
    `idDepartment`  INT(11)         NOT NULL    AUTO_INCREMENT,
    `name`          VARCHAR(45)     NOT NULL,
    `address`       VARCHAR(45)     NOT NULL,
    `phone`         BIGINT(20)      NOT NULL,
    `email`         VARCHAR(45)     NOT NULL,
    PRIMARY KEY (`idDepartment`)
);

CREATE TABLE `HasSubDepartment` (
    `idDepartment`  INT(11)         NOT NULL,
    `idSubDepartment`   INT(11)         NOT NULL,
    FOREIGN KEY (`idDepartment`)
        REFERENCES `Department` (`idDepartment`)
        ON DELETE CASCADE,
    FOREIGN KEY (`idSubDepartment`)
        REFERENCES `Department` (`idDepartment`)
        ON DELETE CASCADE
);

CREATE TABLE `HasDepartment` (
    `idDepartment`  INT(11)         NOT NULL,
    `idProfile`     INT(11)         NOT NULL,
    FOREIGN KEY (`idDepartment`)
        REFERENCES `Department` (`idDepartment`)
        ON DELETE CASCADE,
    FOREIGN KEY (`idProfile`)
        REFERENCES `Profile` (`idProfile`)
        ON DELETE CASCADE
);

CREATE TABLE `MediaDepartmentAccess` (
    `idDepartment`  INT(11)         NOT NULL,
    `idMedia`       INT(11)         NOT NULL,
    FOREIGN KEY (`idDepartment`)
        REFERENCES `Department` (`idDepartment`)
        ON DELETE CASCADE,
    FOREIGN KEY (`idMedia`)
        REFERENCES `Media` (`idMedia`)
        ON DELETE CASCADE
);
