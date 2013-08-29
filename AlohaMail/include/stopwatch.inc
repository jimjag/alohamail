<?php

class stopwatch{
	var $entries;
	var $buffer;
	var $show_mem;
	
	function stopwatch($buffer=false){
		$this->entries = array();
		$this->buffer = $buffer;
		$this->show_mem = function_exists('memory_get_usage');
		if ($this->buffer) ob_start();
	}
	
	function register($message){
		$entry_a["time"] = microtime();
		$entry_a["message"] = $message.($this->show_mem?'['.memory_get_usage().']':'');
		array_push($this->entries, $entry_a);
	}
	
	function purge(){
		if ($this->buffer) ob_clean();
	}
	
	function dump(){
		if ($this->buffer){
			$contents = ob_get_contents();
			ob_end_clean();
			$this->register("pre flush");
			echo $contents;
			//echo ob_get_clean();
			flush();
			//$this->register("flushed");
			$this->register("flushed ".strlen($contents)."B");
		}
		
		reset($this->entries);
		while ( list($k, $blah) = each($this->entries) ){
			$a = $this->entries[$k];
			list($usec, $sec)=explode(" ", $a["time"]);
			$t = (float)$sec + (float)$usec;
			$diff = ($prev_t?($t-$prev_t):0.0);
			printf("%2.4f\t%2.4f\t%s\n", $t, $diff, $a["message"]);
			//echo $t."\t".$a["message"].($prev_t?"\t[".($t - $prev_t)."]":"")."\n";
			$prev_t = $t;
		}
		$this->total();
	}
	
	function total(){
		$c = count($this->entries);
		
		list($usec, $sec)=explode(" ", $this->entries[0]["time"]);
		$start = (float)$sec + (float)$usec;
		
		list($usec, $sec)=explode(" ", $this->entries[$c-1]["time"]);
		$finish = (float)$sec + (float)$usec;
		
		echo "Total: ".($finish-$start)."\n";
	}
	
	function now(){
		$str = microtime();
		list($usec, $sec)=explode(" ", $str);
		return (float)$sec + (float)$usec;
	}
}

?>