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
	case 'delete_category':
		DeleteCategory();
		break;
	case 'list_categories':
		ListCategories();
		break;
	case 'rename_category':
		RenameCategory();
		break;
	default:
		$json_out['status'] = 'fail';
		$json_out['error_code'] = 'Unrecognized action';
		echo json_encode($json_out);
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

function DeleteCategory()
{
	global $g_dbconn;
	$catid = ReadPOSTInt('catid', '-1');
	
	$result = $g_dbconn->query('delete from deviceCategory where deviceCategory_id = \'' . $catid . '\' limit 1');
	if(!$result)
	{
		$json_out['status'] = 'fail';
		$json_out['error_code'] = DatabaseError();
	}
	
	else
		$json_out['status'] = 'ok';
	
	echo json_encode($json_out);
}

function ListCategories()
{
	global $g_dbconn;
	$result = $g_dbconn->query('select * from deviceCategory where parent_id is NULL');
	$json_out = array();
	if(!$result)
	{
		$json_out['status'] = 'fail';
		$json_out['error_code'] = DatabaseError();
		die(json_encode($json_out));
	}
	
	$json_out['cats'] = array();
	while($row = $result->fetch_object())
		array_push($json_out['cats'], DumpCategory($row));
			
	$json_out['status'] = 'ok';
	echo json_encode($json_out);
}

function DumpCategory($row)
{
	$ret = array();
	$ret['name'] = $row->name;
	$ret['id'] = intval($row->deviceCategory_id);
	
	global $g_dbconn;
	$result = $g_dbconn->query('select * from deviceCategory where parent_id = \'' . $row->deviceCategory_id . '\'');
	if(!$result)
	{
		$ret['status'] = 'fail';
		$ret['error_code'] = DatabaseError();
		die(json_encode($ret));
	}
	
	$ret['children'] = array();
	while($r = $result->fetch_object())
		array_push($ret['children'], DumpCategory($r));
	
	return $ret;
}

function RenameCategory()
{
	global $g_dbconn;
	$catid = ReadPOSTInt('catid', '-1');
	$name = DatabaseSanitize(ReadPOSTString('name',''));
	
	$result = $g_dbconn->query('update deviceCategory set name = \'' . $name . '\' where deviceCategory_id = \'' . $catid . '\' limit 1');
	if(!$result)
	{
		$json_out['status'] = 'fail';
		$json_out['error_code'] = DatabaseError();
	}
	
	else
		$json_out['status'] = 'ok';
	
	echo json_encode($json_out);
}
?>
