-- @file cleanup.sql
-- @author Andrew D. Zonenberg
-- @brief Deletes pretty much all content from the database and recreates table structure.
-- You almost certainly do NOT want to use this in a production environment except for initial setup.

-- Create users table and add default admin:password account
drop table if exists users;
create table users(
	`user_id` int auto_increment not null,
	`name` varchar(255) not null,
	`password_hash` varchar(255) not null,
	primary key(`user_id`)
	);
insert into users (`name`, `password_hash`) values('admin', '$1$WJoO5qrn$aNQHsgwaUETkgNxuK.o0A0');

drop table if exists manufacturer;
create table manufacturer(
	`manufacturer_id` int auto_increment not null,
	`name` varchar(255) not null,
	`website` varchar(255),
	`logo_url` varchar(255),
	primary key(`manufacturer_id`)
	);

drop table if exists datasheet;
create table datasheet(
	`datasheet_id` int auto_increment not null,
	`title` varchar(255) not null,
	`url` varchar(255) not null,
	`local_path` varchar(255),
	primary key(`datasheet_id`)
	);

drop table if exists device;
create table device(
	`device_id` int auto_increment not null,
	`name` varchar(255) not null,
	`manufacturer_id` int not null,
	`category_id` int not null,
	foreign key(manufacturer_id) references manufacturer(manufacturer_id) on delete restrict,
	primary key(`device_id`)
	);

drop table if exists datasheetDevices;
create table datasheetDevices(
	`datasheet_id` int auto_increment not null,
	`device_id` int not null,
	foreign key(datasheet_id) references datasheet(datasheet_id) on delete cascade,
	foreign key(device_id) references device(device_id) on delete cascade
	);

drop table if exists enumeration;
create table enumeration(
	`enumeration_id` int auto_increment not null,
	`name` varchar(255) not null,
	primary key(`enumeration_id`)
	);
	
drop table if exists enumeratedValues;
create table enumeratedValues(
	`enumeration_id` int not null,
	`item_value` int not null,
	`item_name` varchar(255) not null
	);

-- TODO: categories and properties

-- Create package class table and populate default classes
drop table if exists packageClass;
create table packageClass(
	`packageClass_id` int auto_increment not null,
	`name` varchar(255) not null,
	primary key(`packageClass_id`)
	);
insert into packageClass values('1', 'DIP');
insert into packageClass values('2', 'SOIC');
insert into packageClass values('3', 'SOP');
insert into packageClass values('4', 'QFP');
insert into packageClass values('5', 'LGA');
insert into packageClass values('6', 'BGA');
insert into packageClass values('7', 'PTH passive');
insert into packageClass values('8', 'SMT passive');
insert into packageClass values('9', 'SOT');
insert into packageClass values('10', 'TO');
insert into packageClass values('11', 'SMT connector');
insert into packageClass values('12', 'PTH connector');
insert into packageClass values('13', 'Mechanical hardware');
insert into packageClass values('14', 'Tool');
insert into packageClass values('15', 'Liquid');

-- Create package table and populate default packages
drop table if exists package;
create table package(
	`package_id` int auto_increment not null,
	`shortname` varchar(32) not null,
	`name` varchar(64) not null,
	`pincount` int not null,
	`packageClass_id` int not null,
	`image_url` varchar(255),
	lead_pitch_um int not null,
	foreign key(`packageClass_id`) references packageClass(`packageClass_id`) on delete restrict,
	primary key(`package_id`)
	);
insert into package values('1', 'PDIP8', 'PDIP-8, 0.3 inch width', '8', '1', 'http://media.digikey.com/Renders/~~Pkg.Case%20or%20Series/8-DIP.jpg', '2540');

drop table if exists physicalDevice;
create table physicalDevice(
	`physicalDevice_id` int auto_increment not null,
	`suffix` varchar(64),
	`package_id` int not null,
	`image_url` varchar(255), 
	`device_id` int not null,
	`number_in_stock` int not null,
	`min_qty` int not null,
	foreign key(`package_id`) references package(`package_id`) on delete restrict,
	foreign key(`device_id`) references device(`device_id`) on delete cascade,
	primary key(`physicalDevice_id`)
	);

drop table if exists bom;
create table bom(
	`bom_id` int auto_increment not null,
	`title` varchar(255) not null,
	`owner_id` int not null,
	`public_bom` int not null,
	foreign key(`owner_id`) references users(`user_id`) on delete cascade,
	primary key(`bom_id`)
	);

drop table if exists bomEntry;
create table bomEntry(
	`bom_id` int not null,
	`physicalDevice_id` int not null,
	`qty` int not null,
	`comments` varchar(255),
	foreign key(`bom_id`) references bom(`bom_id`) on delete cascade,
	foreign key(`physicalDevice_id`) references physicalDevice(`physicalDevice_id`) on delete cascade
	);
