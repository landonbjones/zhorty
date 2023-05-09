#!/usr/bin/php
<?php
$output = array();
exec("ps a -e",$output);
$num_to_kill = 0;
$num_running = 0;
if ($argc == 1) {
	$num_to_kill = 0;
} else if (is_numeric($argv[1])) {
	$num_to_kill = $argv[1];
} elseif ($argv[1] == 'all') {
	$num_to_kill = 1000;
} 
$killed = 0;
foreach ($output as $line) {
	if (strstr($line,"shorty_gen")) {
		$data = explode(" ", $line);
		$i = 0;
		while (!is_numeric($data[$i])) {
			$i++;
		} 
		$pid = $data[$i];
		if ($num_to_kill > $killed) {
			exec("kill -s 3 " . $pid);
			echo $pid . " was killed.\n";
			usleep(700000);
			$killed++;
		} else {
			$num_running++;
		}
	}
}
if ($num_to_kill == 1000) {
	sleep(2);
}

if ($num_running == 1) {
	print $num_running . " generators is running\n";
} else if ($num_running > 0) {
	print $num_running . " generators are running\n";
} else  {
	print "No generators are running\n";
}
?>
