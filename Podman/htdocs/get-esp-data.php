<?php

$servername = "127.0.0.1";

// REPLACE with your Database name
$dbname = "IOT";
// REPLACE with Database user
$username = "IOT";
// REPLACE with Database user password
$password = "JeJSFgM0g9vdJrdn";

function StringInputCleaner($data) // NOTE: source https://www.digi77.com/ways-to-sanitize-data-and-prevent-sql-injections-in-php/
{
	//remove space bfore and after
	$data = trim($data);
	//remove slashes
	$data = stripslashes($data);
	$data = (filter_var($data, FILTER_SANITIZE_STRING));
	return $data;
}

if ($_SERVER["REQUEST_METHOD"] == "GET") {

	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	if ($_GET["selector"] == '') {
		die("No selector given");
	}
	/*
		$query = "CREATE PROCEDURE fill_calendar()
      		 BEGIN
			   	TRUNCATE TABLE calendar;
				DECLARE crt_date DATETIME;
       			SET crt_date=timestampadd(DAY, -1, now());
       			WHILE crt_date <= now() DO INSERT INTO calendar VALUES(DATE_FORMAT(crt_date, '%Y-%m-%d %H:%i'));
					SET crt_date = timestampadd(MINUTE, 5, crt_date); 
       		END WHILE;
    	END";
		$conn->query($query);
		*/
	if ($_GET["selector"] == "0") {
		$sql = "select floor(avg(value)), reading_time, floor(timestampdiff(MINUTE, current_time(), reading_time) / 5) * 5 from LevelSensor where reading_time >= timestampadd(DAY, -1, now()) group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 5) * 5 order by reading_time desc";
		$filter = "floor(timestampdiff(MINUTE, current_time(), reading_time) / 5) * 5";
		$difference = 5;
	} elseif ($_GET["selector"] == "1") {
		$sql = "select floor(avg(value)), reading_time, floor(timestampdiff(MINUTE, current_time(), reading_time) / 35) * 35 from LevelSensor where reading_time >= timestampadd(WEEK, -1, now()) group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 35) * 35 order by reading_time desc";
		$filter = "floor(timestampdiff(MINUTE, current_time(), reading_time) / 35) * 35";
		$difference = 35;
	} elseif ($_GET["selector"] == "2") {
		$sql = "select floor(avg(value)), reading_time, floor(timestampdiff(MINUTE, current_time(), reading_time) / 155) * 155 from LevelSensor where reading_time >= timestampadd(MONTH, -1, now()) group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 155) * 155 order by reading_time desc";
		$filter = "floor(timestampdiff(MINUTE, current_time(), reading_time) / 155) * 155";
		$difference = 155;
	} elseif ($_GET["selector"] == "3") {
		$sql = "select floor(avg(value)), reading_time, floor(timestampdiff(MINUTE, current_time(), reading_time) / 1775) * 1775 from LevelSensor where reading_time >= timestampadd(YEAR, -1, now()) group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 1775) * 1775 order by reading_time desc";
		$filter = "floor(timestampdiff(MINUTE, current_time(), reading_time) / 1775) * 1775";
		$difference = 1775;
	}
	else {
		$conn->close();
		echo "no selector given";
		return;
	}

	//$sql = "select id, reading_time, avg(value), floor(timestampdiff(MINUTE, current_time(), reading_time) / 60) * 60 from LevelSensor group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 60) * 60";

	$result = $conn->query($sql);
	//if ($result->num_rows > 0) {
		// output data of each row
		$check = 0;
		$row = mysqli_fetch_assoc($result);
		while ($check >= -(288 * $difference)) {
			if ($row[$filter] == $check) {
				echo $row["floor(avg(value))"];
				$row = mysqli_fetch_assoc($result);
			}
			echo ",";
			$check = $check - $difference;
		}
		/*
		echo StringInputCleaner($row["floor(avg(value))"]);
		while ($row = mysqli_fetch_assoc($result)) {
			echo "," . StringInputCleaner($row["floor(avg(value))"]);
		}
		
	} else {
		echo "0 results";
	}*/
	$conn->close();
}
?>
