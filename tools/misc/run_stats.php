#!/usr/bin/php
<?php
$system = gethostname();
$dir = getcwd() . "/";

if ($argc == 1) {
	$num_to_start = 1;
} else if (is_numeric($argv[1])) {
	$num_to_start = $argv[1];
} else {
	echo "Bad number to start.\n";
	exit;
}
date_default_timezone_set("America/Chicago");
$log_base =  $dir . "logs/" .date("Y_m_d_H_i_s");
$file_base =  $dir . "temp/" .date("Y_m_d_H_i_s");
for ($i = 1;$i <= $num_to_start; $i++) {
	$log = $log_base . "_" . sprintf('%02d',$i) . ".txt";
	$file = $file_base . "_" . sprintf('%02d',$i);
	exec($dir . "shorty_stats " . $file . " > " . $log . " 2>&1 &");
	usleep(700000);
}
?>
