<?php
/////////////////////////////////////////////////////////
//	
//	include/cache.FS.inc
//
//	(C)Copyright 2003 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail, and released under GPL.
//	See COPYING, or http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////
/********************************************************
	PURPOSE: Unified interface to read/write cache

********************************************************/

include_once("../include/fs_path.inc");

$CACHE_ERROR = '';

function cache_read_data($path){
	global $CACHE_ERROR;
	
	//open file
	$fp = fopen($path, "r");
	if (!$fp){
		$CACHE_ERROR = '(Cache.FS): Could not open file "'.$path.'" for reading.';
		return false;
	}
	
	//read data
	$data = false;
	$data = @fread($fp, filesize($path));
	if ($data){
		$data = unserialize($data);
	}else if ($data===false) $CACHE_ERROR = '(Cache.FS): Error reading from "'.$path.'"';
	else if (empty($data)) $CACHE_ERROR = '(Cache.FS): File "'.$path.'" contains no data';
	fclose($fp);

	return $data;
}

function cache_read($user, $host, $key){
	global $CACHE_ERROR;
	
	$key = urlencode($key);
	
	//check if file is there
	$user_dir = fs_get_path("cache", $user, $host);
	if (!$user_dir){
		$CACHE_ERROR = '(Cache.FS): Directory "'.$user_dir.'" does not exist.';
		return false;
	}
	$path = $user_dir.$key;
	if (!@file_exists(realpath($path))){
		$CACHE_ERROR = '(Cache.FS): Cache file "'.$path.'" does not exist.';
		return false;
	}
	
	//read data into array
	$data = cache_read_data($path);	
	
	if (!$data) return false;
	else return $data["_d"];
}

function cache_write($user, $host, $key, $data, $volatile=true){
	global $CACHE_ERROR;
	
	$key = urlencode($key);
	
	//format cache file path	
	$user_dir = fs_get_path("cache", $user, $host);
	if (!$user_dir){
		$CACHE_ERROR = '(Cache.FS): Directory "'.$user_dir.'" does not exist.';
		return false;
	}
	$path = $user_dir.$key;
	
	//open file for writing
	$fp = @fopen($path, "w");
	if (!$fp){
		$CACHE_ERROR = '(Cache.FS): Could not open file "'.$path.'" for writing.';
		return false;
	}
	
	//package data
	$package["_s"] = ($volatile?"0":"1");
	$package["_d"] = $data;
	
	//write data
	fputs($fp, serialize($package));
	
	fclose($fp);
	
	return true;
}

function cache_clear($user, $host, $key){
	$key = urlencode($key);
	
	//format cache file path	
	$user_dir = fs_get_path("cache", $user, $host);
	if (!$user_dir) return false;
	$path = $user_dir.$key;

	//check if file is there
	if (!@file_exists(realpath($path))) return false;
	else return unlink($path);
}

function cache_clear_all($user, $host){	
	//delete cache files
	$cacheDir = fs_get_path("cache", $user, $host);
	if (@is_dir(realpath($cacheDir))){
		if ($handle = opendir($cacheDir)) {
			while (false !== ($file = readdir($handle))) { 
				if ($file != "." && $file != "..") { 
					$file_path = $cacheDir."/".$file;
					$data = cache_read_data($file_path);
					if (!$data["_s"]) unlink($file_path);
				} 
			}
			closedir($handle); 
		}
	}
}

?>