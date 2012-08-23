<?php
/**
	@file penguinserver.php
	@author Andrew D. Zonenberg
	@brief Main web service for the Penguin server
 */
header('Content-Type: application/json');
require_once('../inc/util.inc.php');

function main()
{
	$action = ReadGETString('action', '');
	switch($action)
	{
	case 'add_category':
		AddCategory();
		break;
	}
}

function AddCategory()
{
	global $g_dbconn;
	
	$catname = DatabaseSanitize(ReadPOSTString('catname',''));
	$parent = ReadPOSTInt('parent', -1);
	$json_out = array();
	
	if($catname == '')
	{
		$json_out['status'] = 'fail';
		$json_out['error_code'] = 'No category name specified';
	}
	else
	{
		//Add the row
		$result = $g_dbconn->query('insert into deviceCategory (`name`, `parent_id`) values(\'' . $catname . '\', ' . (($parent<0)?'NULL':('\'' . $parent . '\'')) . ');');
		if(!$result)
		{
			$json_out['status'] = 'fail';
			$json_out['error_code'] = DatabaseError();
		}
		
		else
		{
			$json_out['status'] = 'ok';
			$json_out['catid'] = $g_dbconn->insert_id;
		}
	}
	
	echo json_encode($json_out);
}
?>
