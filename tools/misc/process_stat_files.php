#!/usr/bin/php
<?php
$dir = "/Users/lbj/Projects/shorty-c/";
$db = "zhorty";
$user = "root";
$password = "";

#Make Database connection
$conn = mysqli_connect ("localhost", $user, $password) or die ('I cannot connect to the database because ' . mysql_error() . "\n");
mysqli_select_db ($conn, $db) or die ('I cannot select the database.\n');

$from_dir = $dir . "hands/";
$new_dir = $dir . "processed_hands/";
$zipped_dir = $dir . "zipped_hands/";

exec("ls " . $from_dir . "*.sql",$files);
$count = 0;
$file_list = "";

foreach ($files as $file) {
	$log = "";
	$file_parts = explode("/",$file);
	$file_name = $file_parts[sizeof($file_parts)-1];

	exec("mysql -uroot " . $db . " < " . $from_dir . $file_name);
	$sql = "select count(*) from game_stats";
	$hands = get_total($sql);
	if ($hands > 0) {
		$sql = "select sum(solve_time) from game_stats";
		$res = query($sql);
		$row = mysqli_fetch_assoc($res);
		$solve_time = floatval($row['sum(solve_time)']);

		$sql = "select count(*) from game_stats where winable = false";
		$unwinable_hands = get_total($sql);

		$sql = "select sum(solve_time) from game_stats where winable = false";
		$res = query($sql);
		$row = mysqli_fetch_assoc($res);
		$unwinable_solve_time = floatval($row['sum(solve_time)']);

		$sql = "select count(*) from game_stats where winable and win_percent < .001";
		$hard_hands = get_total($sql);

		$sql = "select count(*) from game_stats where winable and win_percent < .0000000001";
		$challange_hands = get_total($sql);

		$sql = "select min(win_percent) from game_stats where winable";
		$res = query($sql);
		$row = mysqli_fetch_assoc($res);
		$lowest_win_percent = floatval($row['min(win_percent)']);

		$sql = "insert into file_stats values('" . $file_name . "'," . $hands . "," . $solve_time . "," . $unwinable_hands . "," . $unwinable_solve_time . "," . $hard_hands  . "," . $challange_hands . "," . $lowest_win_percent . ")";
		$res = query($sql);

		$sql = "delete from game_stats where winable = false";
		$res = query($sql);

		exec("mysqldump -uroot $db game_stats > $zipped_dir" . $file_name);
		exec("gzip $zipped_dir" . $file_name);

		$sql = "delete from game_stats";
		$res = query($sql);
		
		exec("mv " . $from_dir . $file_name . " " . $new_dir . "/.");
	}
}

function query($sql) {
	global $conn;
	$backtrace = debug_backtrace();
	$file_name = $backtrace[0]['file'];
	$line_number = $backtrace[0]['line'];
	$res = mysqli_query($conn,$sql);
	if (!$res) {
		print "<b>" . mysqli_error() . "</b><br>\n" . $sql . "<br>\nin " . $file_name . " on line " . $line_number . "\n";
		exit;
	}
	return $res;
}

function get_total($sql) {
	$res = query($sql);
	$row = mysqli_fetch_assoc($res);
	return intval($row['count(*)']);
}

?>