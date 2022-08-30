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

$api_key = $UL1 = $UL2 = $UL3 = $IL1 = $IL2 = $IL3 = $Pcl = $Pch = $Ppl = $Pph = $Pp = $Pc = $Vg = $Vw = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $UL1 = floatval(str_replace(',', '.', test_input($_POST["UL1"])));
        $UL2 = floatval(str_replace(',', '.', test_input($_POST["UL2"])));
        $UL3 = floatval(str_replace(',', '.', test_input($_POST["UL3"])));
        $IL1 = floatval(str_replace(',', '.', test_input($_POST["IL1"])));
        $IL2 = floatval(str_replace(',', '.', test_input($_POST["IL2"])));
        $IL3 = floatval(str_replace(',', '.', test_input($_POST["IL3"])));
        $Pcl = floatval(str_replace(',', '.', test_input($_POST["Pcl"])));
        $Pch = floatval(str_replace(',', '.', test_input($_POST["Pch"])));
        $Ppl = floatval(str_replace(',', '.', test_input($_POST["Ppl"])));
        $Pph = floatval(str_replace(',', '.', test_input($_POST["Pph"])));
        $Pp = floatval(str_replace(',', '.', test_input($_POST["Pp"])));
        $Pc = floatval(str_replace(',', '.', test_input($_POST["Pc"])));
        $Vg = floatval(str_replace(',', '.', test_input($_POST["Vg"])));
        $Vw = floatval(str_replace(',', '.', test_input($_POST["Vw"])));
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO FluviusMeter (UL1, UL2, UL3, IL1, IL2, IL3, Pcl, Pch, Ppl, Pph, Pp, Pc, Vg, Vw)
        VALUES (" . $UL1 . ", " . $UL2 . ", " . $UL3 . ", " . $IL1 . ", " . $IL2 . ", " . $IL3 . ", " . $Pcl . ", " . $Pch . ", " . $Ppl . ", " . $Pph . ", " . $Pp . ", " . $Pc . ", " . $Vg . ", " . $Vw . ")";

        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}

?>