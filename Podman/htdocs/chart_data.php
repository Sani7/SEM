<?php
//address of the server where db is installed
$servername = "127.0.0.1";

//username to connect to the db
//the default value is root
$username = "IOT";

//password to connect to the db
//this is the value you would have specified during installation of WAMP stack
$password = "JeJSFgM0g9vdJrdn";

//name of the db under which the table is created
$dbName = "IOT";

//establishing the connection to the db.
$conn = new mysqli($servername, $username, $password, $dbName);

//checking if there were any error during the last connection attempt
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

//the SQL query to be executed
$query = "SELECT reading_time, UL1 FROM FluviusMeter";

//storing the result of the executed query
$result = $conn->query($query);

//initialize the array to store the processed data
$jsonArray = array();

//check if there is any data returned by the SQL Query
if ($result->num_rows > 0) {
  //Converting the results into an associative array
  while($row = $result->fetch_assoc()) {
    $jsonArrayItem = array();
    $jsonArrayItem['label'] = $row['reading_time'];
    $jsonArrayItem['value'] = $row['UL1'];
    //append the above created object into the main array.
    array_push($jsonArray, $jsonArrayItem);
  }
}

//Closing the connection to DB
$conn->close();

//set the response content type as JSON
header('Content-type: application/json');
//output the return value of json encode using the echo function. 
echo json_encode($jsonArray);
?>
