# Name the database sanely
USE `savannah`;

# Remove old tables, we like clean uninstall
DROP TABLE IF EXISTS `MediaDepartmentAccess`;
DROP TABLE IF EXISTS `HasDepartment`;
DROP TABLE IF EXISTS `HasSubDepartment`;
DROP TABLE IF EXISTS `Department`;
DROP TABLE IF EXISTS `HasLink`;
DROP TABLE IF EXISTS `MediaProfileAccess`;
DROP TABLE IF EXISTS `HasTag`;
DROP TABLE IF EXISTS `Tags`;
DROP TABLE IF EXISTS `Media`;
DROP TABLE IF EXISTS `ListOfApps`;
DROP TABLE IF EXISTS `Apps`;
DROP TABLE IF EXISTS `HasGuardian`;
DROP TABLE IF EXISTS `Profile`;
DROP TABLE IF EXISTS `AuthUsers`;

# Kill the database
DROP DATABASE `savannah`;
