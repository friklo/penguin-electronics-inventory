-- @file cleanup.sql
-- @author Andrew D. Zonenberg
-- @brief Deletes pretty much all content from the database and recreates table structure.
-- You almost certainly do NOT want to use this in a production environment except for initial setup.

-- TODO: Create indexes!!!
-- The current schema is purely structural and has no indexing whatsoever.

-- Disable foreign key checks during table creation
-- or self-referential FK creation may fail
set foreign_key_checks = 0 ;

-- Need to drop tables in reverse order
drop table if exists priceQuote;
drop table if exists distributorDevice;
drop table if exists distributorPackage;
drop table if exists distributor;
drop table if exists bomEntry;
drop table if exists bom;
drop table if exists physicalDevice;
drop table if exists packageType;
drop table if exists packageClass;
drop table if exists propertyValue;
drop table if exists datasheetDevice;
drop table if exists device;
drop table if exists deviceCategory_properties;
drop table if exists propertyDefinition;
drop table if exists deviceCategory;	
drop table if exists enumeratedValues;
drop table if exists enumeration;
drop table if exists datasheet;
drop table if exists manufacturer;
drop table if exists users;

-- Create users table and add default admin:password account
create table users(
	`user_id` int auto_increment not null,
	`name` varchar(255) not null,
	`password_hash` varchar(255) not null,
	primary key(`user_id`)
	) engine=InnoDB;
insert into users (`name`, `password_hash`) values('admin', '$1$WJoO5qrn$aNQHsgwaUETkgNxuK.o0A0');

create table manufacturer(
	`manufacturer_id` int auto_increment not null,
	`name` varchar(255) not null,
	`website` varchar(255),
	`logo_url` varchar(255),
	primary key(`manufacturer_id`)
	) engine=InnoDB;

create table datasheet(
	`datasheet_id` int auto_increment not null,
	`title` varchar(255) not null,
	`url` varchar(255) not null,
	`local_path` varchar(255),
	primary key(`datasheet_id`)
	) engine=InnoDB;

create table enumeration(
	`enumeration_id` int auto_increment not null,
	`name` varchar(255) not null,
	primary key(`enumeration_id`)
	) engine=InnoDB;

create table enumeratedValues(
	`enumeration_id` int not null,
	`item_value` int not null,
	`item_name` varchar(255) not null
	) engine=InnoDB;

create table deviceCategory(
	`deviceCategory_id` int auto_increment not null,
	`name` varchar(255) not null,
	`parent_id` int,
	foreign key(`parent_id`) references deviceCategory(`deviceCategory_id`) on delete cascade,
	primary key(`deviceCategory_id`)
	) engine=InnoDB;

create table propertyDefinition(
	`propertyDefinition_id` int auto_increment not null,
	`name` varchar(255) not null,
	`units` int not null,
	`enum_id` int,
	foreign key(`enum_id`) references enumeration(`enumeration_id`) on delete restrict,
	primary key(`propertyDefinition_id`)
	) engine=InnoDB;

create table deviceCategory_properties(
	`deviceCategory_id` int not null,
	`propertyDefinition_id` int not null,
	foreign key(`deviceCategory_id`) references deviceCategory(`deviceCategory_id`) on delete cascade,
	foreign key(`propertyDefinition_id`) references propertyDefinition(`propertyDefinition_id`) on delete cascade
	) engine=InnoDB;

create table device(
	`device_id` int auto_increment not null,
	`name` varchar(255) not null,
	`manufacturer_id` int not null,
	`deviceCategory_id` int not null,
	foreign key(manufacturer_id) references manufacturer(`manufacturer_id`) on delete restrict,
	foreign key(`deviceCategory_id`) references deviceCategory(`deviceCategory_id`) on delete restrict,
	primary key(`device_id`)
	) engine=InnoDB;
	
create table datasheetDevice(
	`datasheet_id` int auto_increment not null,
	`device_id` int not null,
	foreign key(datasheet_id) references datasheet(datasheet_id) on delete cascade,
	foreign key(device_id) references device(device_id) on delete cascade
	) engine=InnoDB;

create table propertyValue(
	`device_id` int not null,
	`propertyDefinition_id` int not null,
	`integer_value` int,
	`float_value` float,
	`enumerated_value` int,
	foreign key(`device_id`) references device(`device_id`) on delete cascade,
	foreign key(`propertyDefinition_id`) references propertyDefinition(`propertyDefinition_id`) on delete cascade
	-- TODO: figure out how to do enumerated foreign key constraints
	) engine=InnoDB;

-- Create package class table and populate default classes
create table packageClass(
	`packageClass_id` int auto_increment not null,
	`name` varchar(255) not null,
	index (`packageClass_id`),
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
create table packageType(
	`packageType_id` int auto_increment not null,
	`shortname` varchar(32) not null,
	`name` varchar(64) not null,
	`pincount` int not null,
	`packageClass_id` int not null,
	`image_url` varchar(255),
	lead_pitch_um int not null,
	index (`packageType_id`),
	foreign key(`packageClass_id`) references packageClass(`packageClass_id`) on delete restrict,
	primary key(`packageType_id`)
	) engine=InnoDB;
insert into packageType values('1', 'PDIP8', 'PDIP-8, 0.3 inch width', '8', '1', 'http://media.digikey.com/Renders/~~Pkg.Case%20or%20Series/8-DIP.jpg', '2540');

create table physicalDevice(
	`physicalDevice_id` int auto_increment not null,
	`packageType_id` int not null,
	`suffix` varchar(64),
	`package_id` int not null,
	`image_url` varchar(255), 
	`device_id` int not null,
	`number_in_stock` int not null,
	`min_qty` int not null,
	foreign key(`packageType_id`) references packageType(`packageType_id`) on delete restrict,
	foreign key(`device_id`) references device(`device_id`) on delete cascade,
	primary key(`physicalDevice_id`)
	) engine=InnoDB;

create table bom(
	`bom_id` int auto_increment not null,
	`title` varchar(255) not null,
	`owner_id` int not null,
	`public_bom` int not null,
	foreign key(`owner_id`) references users(`user_id`) on delete cascade,
	primary key(`bom_id`)
	) engine=InnoDB;

create table bomEntry(
	`bom_id` int not null,
	`physicalDevice_id` int not null,
	`qty` int not null,
	`comments` varchar(255),
	foreign key(`bom_id`) references bom(`bom_id`) on delete cascade,
	foreign key(`physicalDevice_id`) references physicalDevice(`physicalDevice_id`) on delete cascade
	) engine=InnoDB;

create table distributor(
	`distributor_id` int auto_increment not null,
	`name` varchar(255) not null,
	`website` varchar(255) not null,
	`logo_url` varchar(255),
	primary key(`distributor_id`)
	) engine=InnoDB;

create table distributorPackage(
	`distributorPackage_id` int auto_increment not null,
	`name` varchar(255) not null,
	primary key(`distributorPackage_id`)
	);
insert into distributorPackage values('1', 'Bulk');
insert into distributorPackage values('2', 'Tube');
insert into distributorPackage values('3', 'Tray');
insert into distributorPackage values('4', 'Tape and Reel');
insert into distributorPackage values('5', 'Cut Tape');

create table distributorDevice(
	`distributorDevice_id` int auto_increment not null,
	`distributor_id` int not null,
	`distributor_partnum` varchar(255) not null,
	`distributor_url` varchar(255) not null,
	`distributorPackage_id` int not null,
	`physicalDevice_id` int not null,
	foreign key(`distributor_id`) references distributor(`distributor_id`) on delete cascade,
	foreign key(`physicalDevice_id`) references physicalDevice(`physicalDevice_id`) on delete cascade,
	foreign key(`distributorPackage_id`) references distributorPackage(`distributorPackage_id`) on delete restrict,
	primary key(`distributorDevice_id`)
	) engine=InnoDB;

create table priceQuote(
	`distributorDevice_id` int auto_increment not null,
	`qty` int not null,
	`price` int not null,
	foreign key(`distributorDevice_id`) references distributorDevice(`distributorDevice_id`) on delete cascade
	) engine=InnoDB;

-- Foreign key checks on from now on
set foreign_key_checks = 1;
