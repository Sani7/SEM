<?php

$servername = "127.0.0.1";

// REPLACE with your Database name
$dbname = "IOT";
// REPLACE with Database user
$username = "IOT";
// REPLACE with Database user password
$password = "JeJSFgM0g9vdJrdn";
// If you change this value, the API key in the sketch needs to match
$api_key_value = "uHKSk7uB29Up3";

$api_key = $value = "";

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

    if ($_GET["selector"] == "0") {
        $sql = "select floor(avg(value)), reading_time, floor(timestampdiff(MINUTE, current_time(), reading_time) / 5) * 5 from DayData where reading_time >= timestampadd(DAY, -1, now()) group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 5) * 5 order by reading_time desc";
        $filter = "floor(timestampdiff(MINUTE, current_time(), reading_time) / 5) * 5";
        $difference = 5;
    } elseif ($_GET["selector"] == "1") {
        $sql = "select floor(avg(value)), reading_time, floor(timestampdiff(MINUTE, current_time(), reading_time) / 35) * 35 from DayData where reading_time >= timestampadd(WEEK, -1, now()) group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 35) * 35 order by reading_time desc";
        $filter = "floor(timestampdiff(MINUTE, current_time(), reading_time) / 35) * 35";
        $difference = 35;
    } elseif ($_GET["selector"] == "2") {
        $sql = "select floor(avg(value)), reading_time, floor(timestampdiff(MINUTE, current_time(), reading_time) / 155) * 155 from DayData where reading_time >= timestampadd(MONTH, -1, now()) group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 155) * 155 order by reading_time desc";
        $filter = "floor(timestampdiff(MINUTE, current_time(), reading_time) / 155) * 155";
        $difference = 155;
    } elseif ($_GET["selector"] == "3") {
        $sql = "select floor(avg(value)), reading_time, floor(timestampdiff(MINUTE, current_time(), reading_time) / 1775) * 1775 from DayData where reading_time >= timestampadd(YEAR, -1, now()) group by floor(timestampdiff(MINUTE, current_time(), reading_time) / 1775) * 1775 order by reading_time desc";
        $filter = "floor(timestampdiff(MINUTE, current_time(), reading_time) / 1775) * 1775";
        $difference = 1775;
    } else {
        $conn->close();
        echo "no selector given";
        return;
    }

    $result = $conn->query($sql);
    $check = 0;
    $row = mysqli_fetch_assoc($result);
    while ($check >= - (288 * $difference)) {
        if ($row[$filter] == $check) {
            echo $row["floor(avg(value))"];
            $row = mysqli_fetch_assoc($result);
        }
        echo ",";
        $check = $check - $difference;
    }

    $conn->close();
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if ($api_key == $api_key_value) {
        $value = test_input($_POST["value"]);

        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        $sql = "INSERT INTO DayData (value)
        VALUES ('" . $value . "')";

        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }

        $conn->close();
    } else {
        echo "Wrong API Key provided.";
    }
} else {
    echo "No data posted with HTTP POST.";
}

function test_input($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
