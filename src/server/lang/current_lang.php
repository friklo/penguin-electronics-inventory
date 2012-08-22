<?php
/**
	@file current_lang.php
	@author Andrew D. Zonenberg
	@brief Pulls in all locale-specific strings
 */

switch($g_config['lang'])
{
case 'en-us':
	require_once('en-us.php');
	break;
	
default:
	require_once('en-us.php');
	break;
}

?>
