<?php
/////////////////////////////////////////////////////////
//	
//	include/data_manager.FS.inc
//
//	(C)Copyright 2003 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail. IlohaMail is free software released 
//	under the GPL license.  See enclosed file COPYING for details, or 
//	see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	PURPOSE:
		Data handling abstraction class.
	PRE-CONDITIONS:
	POST-CONDITIONS:
	COMMENTS:
		For FS based backend, $data will contain all data.

********************************************************/

include_once("../include/sort2darray.inc");
include_once("../include/fs_path.inc");

class DataManager_obj{
	var $user;
	var $host;
	var $storage;		//directory for FS backend, table nmae for DB
	var $location;		//file name for FS backend, user's ID for DB backend
	var $path;
	var $data;			//actually contains
	var $error;
	var $id_counter;
	var $is_sharable;
	
	function initialize($user, $host, $location, $backend){

		$this->error = "";		
		
		$storage = fs_get_path("user", $user, $host);
		if (!$storage){
			$this->error = "Folder $storage does not exist\n";
			return false;
		}

		$location = $location.".dat";
		
		$this->storage = $storage;
		$this->location = $location;
		$this->path = $storage."/".$location;
		$this->user = $user;
		$this->host = $host;
		$this->data = false;
		$this->id_counter = 0;
		return true;
	}
	
	function read(){
		$filePath = $this->path;
		$this->data = false;
		
		$fp = fopen($filePath, "a");		//force create file
		if ($fp) fclose($fp);

		$lines = file($filePath);

		if (is_array($lines)){
			$i=1;
			while ( list($key, $line) = each ($lines) ){
				$a = explode(",", chop($line));
				while ( list($k2, $data) = each($a) ){
					list($type, $string) = explode(":", $data);
					if ($type!="id") $string = base64_decode($string);
					//$string = base64_decode($string);
					$this->data[$i][$type] = $string;
				}
				$this->data[$i]["id"] = $i;
				$i++;
			}
		}else{
			$this->error.= "Failed to read from: $filePath.\n";
			$this->data = array();
		}
		
		return $this->data;
	}
	
	function fetch_id($id){
		$a = $this->read();
		
		if (is_array($a)){
			while(list($k,$entry)=each($a)){
				if ($entry["id"]==$id)
					return $entry;
			}
		}
		
		$this->error .= "Record $id not found\n";
		return false;
	}
	
	function save(){
		$filePath = $this->path;

		$fp = fopen($filePath, "w+");
		if ($fp){
			reset($this->data);
			$i=1;
			while ( list($key, $foo) = each($this->data)){
				$line="id:".$i;
				$this->data[$key]["id"] = $i;
				while ( list($k2, $val) = each($this->data[$key])) 
					if ($k2!="id") $line .= ",".$k2.":".base64_encode($val);
				fputs($fp, $line."\n");
				$i++;
			}
			fclose($fp);
			
			return true;
		}else{
			$this->error = "Couldn't open file \"$filePath\" for writing\n";
			return false;
		}
	}
	
	
	function delete($id){
		if (!$this->data) $this->read();
		$result = array();
		$deleted = false;
		
		reset($this->data);
		while ( list($k,$v) = each($this->data) ){
			if ($this->data[$k]["id"] != $id) $result[$k] = $this->data[$k];
			else $deleted = true;
		}
		if ($deleted){
			$this->data = $result;
			$this->save();
		}else{
			$this->error = "Delete failed: $id not found\n";
		}
		
		return $deleted;
	}
	
	function update($id, $array){
		if (!$this->data) $this->read();
		$updated = false;
		reset($this->data);
		while ( list($k,$v) = each($this->data) ){
			if ($this->data[$k]["id"]==$id){
				$this->data[$k] = $array;
				$updated = true;
			}
		}
		if ($updated) $this->save();
		else $this->error = "Update failed:  $id not found\n";
		
		return $updated;
	}
	
	function insert($array){
		if (!$this->data) $this->read();
		$array["id"] = $this->unique_id();
		$this->data[] = $array;
		if ($this->save()) return $array['id'];
		else return false;
	}
	
	
	function sort($field, $order){
		if (!$this->data) $this->read();
		if (empty($field)) return $this->data;
		$sort_a = explode(",", $field);
		$num_fields = count($sort_a);
		for ($i=$num_fields;$i>0;$i--){
			$this->data = sort2darray($this->data, $sort_a[$i-1], $order);
		}
		return $this->data;
	}
	
	
	function getDistinct($field, $order){
		$this->sort($field, $order);
		
		$index = array();
		$result = array();
		
		reset($this->data);
		while ( list($k,$v) = each($this->data) ){
			$value = $this->data[$k][$field];
			if (empty($index[$value])){
				$index[$value] = 1;
				$result[] = $value;
			}
		}
		
		return $result;
	}
	
	function fetch_fields($fields, $sort_field, $order){
		if ($order) $this->sort($sort_field, $order);
		else if (!$this->data) $this->read();
		
		$field_list = explode(",",$fields);
		if (empty($fields) || !is_array($field_list)) return $this->data;
				
		$row = 0;
		reset($this->data);
		while ( list($k,$v) = each($this->data) ){
			reset($field_list);
			while(list($fnum,$field)=each($field_list)){
				$result[$row][$field] = $this->data[$k][$field];
			}
			$row++;
		}
				
		return $result;
	}
	
	function _compare($val1, $op, $val2){
		if ($val2=="NULL") $val2="";
		switch ($op){
			case 'is':
			case '=':
				return $val1==$val2;
			case 'not':
			case '!=':
				return $val1!=$val2;
			case '<':
				return $val1<$val2;
			case '>':
				return $val1>$val2;
			default:
				return false;
		}
	}

	function fetch_fields_where($fields, $where, $sort_field="", $order=""){
		$data = $this->fetch_fields($fields, $sort_field, $order);
		if (!is_array($data)) return array();
		if (!is_array($where)) return $this->fetch_fields($fields, $sort_fields, $order);
		
		$fields_a = explode(",",$fields);
		
		
		$cmp_field = $where["f"];
		$operator = $where["o"];
		$cmp_val = $where["v"];
		
		$result = array();
		
		reset($this->data);
		$rows=0;
		while(list($k,$vals)=each($this->data)){
			$match = true;
			foreach($where as $w)
				if (!$this->_compare($vals[$w["f"]],$w["o"],$w["v"])) $match=false;
			if (!$match) continue;
			
			foreach($fields_a as $f)
				$result[$rows][$f] = $vals[$f];
			$rows++;
		}
		
		return $result;
	}

	function search($array){
	}
	
	function showError(){
		echo nl2br($this->error);
	}

	function unique_id(){
		$this->id_counter++;
		return time().".".$this->id_counter;
	}
}
?>