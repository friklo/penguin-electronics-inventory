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
	@brief Returns a string parameter from the GET request or a default value if unspecified
	
	@param name				Name of the parameter
	@param default_value	Default value if $_GET[name] is not set
 */
function ReadGETString($name, $default_value)
{
	if(isset($_GET[$name]))
		return $_GET[$name];
	else
		return $default_value;
}

/**
	@brief Returns an integer parameter from the POST request or a default value if unspecified
	
	@param name				Name of the parameter
	@param default_value	Default value if $_POST[name] is not set
 */
function ReadPOSTInt($name, $default_value)
{
	if(isset($_POST[$name]))
		return intval($_POST[$name]);
	else
		return intval($default_value);
}

/**
	@brief Returns a string parameter from the POST request or a default value if unspecified
	
	@param name				Name of the parameter
	@param default_value	Default value if $_POST[name] is not set
 */
function ReadPOSTString($name, $default_value)
{
	if(isset($_POST[$name]))
		return $_POST[$name];
	else
		return $default_value;
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
	Returns a string parameter from the configuration settings or a default value if unspecified
	
	@param name				Name of the parameter
	@param default_value	Default value if $g_config[name] is not set
 */
function ReadConfigString($name, $default_value)
{
	global $g_config;
	if(isset($g_config[$name]))
		return $g_config[$name];
	else
		return $default_value;
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
	@brief Returns a database error string
 */
function DatabaseError()
{
	global $g_dbconn;
	if(ReadConfigInt('verbose_db_errors', 0))
	{
		if($g_dbconn->connect_error)
			return GetLocalizedString('database-error') .  $g_dbconn->connect_error;
		else
			return GetLocalizedString('database-error') .  $g_dbconn->error;
	}
	else
		return GetLocalizedString('generic-error');
}

/**
	@brief Escapes characters that have special meanings in SQL
 */
function DatabaseSanitize($str)
{
	global $g_dbconn;
	return $g_dbconn->real_escape_string($str);
}

/** 
	@brief Set up the database connection and initialize the current session
 */
function SessionInit()
{
	//connect to database
	global $g_dbconn;
	$g_dbconn = new mysqli(	ReadConfigString('db_server','localhost'),
							ReadConfigString('db_user','penguin'),
							ReadConfigString('db_pass',''),
							ReadConfigString('db_name','penguin'),
							ReadConfigInt('db_port',3306));
	if($g_dbconn->connect_error)
		DatabaseError();

	//create session and set up default variables
	session_start();
	if(!isset($_SESSION['uid']))
		$_SESSION['uid'] = -1;
}

// All application code has to be in this function
SessionInit();
main();
if($g_dbconn)
	$g_dbconn->close();
?>
