<?php
/**
	@file util.inc.php
	@author Andrew D. Zonenberg
	@brief Locale-specific strings for EN-US
	
	Try to avoid using HTML or formatting in these strings unless absolutely necessary.
 */
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Error messages
$g_localstrings['generic-error']		= 'Unspecified error';
$g_localstrings['database-error']		= 'Database error: ';
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private strings for setup script
$g_localstrings['setup-confirm']		= 'Are you sure you want to wipe your entire database and start from scratch? If so, please reload this page with "really_sure=1" appended to the URL.';
$g_localstrings['setup-creating-lock']	= 'Creating lock file data/setup.lock...';
$g_localstrings['setup-lock-pointless']	= 'WARNING: The config setting ignore_setup_lock has a value of 1. Anybody can re-run this setup script at any time and wipe out your database!';
$g_localstrings['setup-already-run']	= 'ERROR: The setup script has already been run and ignore_setup_lock is disabled. Aborting!';
$g_localstrings['setup-trampling']		= 'WARNING: The setup script has already been run but ignore_setup_lock is enabled. Destroying the existing database!';
$g_localstrings['setup-done']			= 'Setup completed successfully.';
 
?>
