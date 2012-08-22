<?php
/**
	@file setup.php
	@author Andrew D. Zonenberg
	@brief Sets up initial database structure
 */

require_once('../inc/util.inc.php');

/** 
	@brief Entry point for the setup program
 */
function main()
{
	header('Content-Type: text/plain');
	
	if(ReadGETInt('really_sure', 0))
		DoSetup();

	else
		die(GetLocalizedString('setup-confirm'));
}

/** 
	Run the actual setup process
 */
function DoSetup()
{
	$lock_file_name = '../data/setup.lock';
	
	//Check if the lock file already exists
	if(!file_exists($lock_file_name))
	{
		echo GetLocalizedString('setup-creating-lock') . "\n";
		
		//Warn if ignore_setup_lock is enabled
		if(ReadConfigInt('ignore_setup_lock', '0'))
			echo GetLocalizedString('setup-lock-pointless') . "\n";
			
		//Create the lock file
		touch($lock_file_name);
	}
	else
	{
		//Ignore the lock file and blow away the db
		if(ReadConfigInt('ignore_setup_lock', '0'))
			echo GetLocalizedString('setup-trampling') . "\n";		
		else
			die(GetLocalizedString('setup-already-run')) . "\n";
	}
	
	//If we get to this point the lock either doesn't exist or has been ignored. Start the setup process.
	SessionInit();
	
	//Run the setup queries
	global $g_dbconn;
	if(!$g_dbconn->multi_query(file_get_contents('../setup/setup.sql')))
		DatabaseError();
	while($g_dbconn->more_results())
	{
		if(!$g_dbconn->next_result())
			DatabaseError();
	}
	
	echo GetLocalizedString('setup-done');
}
?>
