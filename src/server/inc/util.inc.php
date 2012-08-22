<?php
/**
	@file util.inc.php
	@author Andrew D. Zonenberg
	@brief Common shared utility functions
 */
 
require_once('../config/config.inc.php');
require_once('../lang/current_lang.php');
 
/**
	@brief Returns an integer parameter from the GET request or a default value if unspecified
	
	@param name				Name of the parameter
	@param default_value	Default value if $_GET[name] is not set
 */
function ReadGETInt($name, $default_value)
{
	if(isset($_GET[$name]))
		return intval($_GET[$name]);
	else
		return intval($default_value);
}

/** 
	Returns an integer parameter from the configuration settings or a default value if unspecified
	
	@param name				Name of the parameter
	@param default_value	Default value if $g_config[name] is not set
 */
function ReadConfigInt($name, $default_value)
{
	global $g_config;
	if(isset($g_config[$name]))
		return intval($g_config[$name]);
	else
		return intval($default_value);
}

/** 
	@brief Returns a localized string
 */
function GetLocalizedString($id)
{
	global $g_localstrings;
	return $g_localstrings[$id];
}

/** 
	Set up the database connection
 */
function ConnectToDatabase()
{
}

// All application code has to be in this function
main();
?>
