-- @file cleanup.sql
-- @author Andrew D. Zonenberg
-- @brief Deletes pretty much all content from the database and recreates table structure.
-- You almost certainly do NOT want to use this in a production environment except for initial setup.

-- Create users table and add default admin:password account
drop table if exists users;
create table users(
	`uid` int auto_increment not null,
	`name` varchar(255) not null,
	`password_hash` varchar(255) not null,
	primary key(`uid`)
	);
insert into users (`name`, `password_hash`) values('admin', '$1$WJoO5qrn$aNQHsgwaUETkgNxuK.o0A0');
