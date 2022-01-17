EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:VCC #PWR?
U 1 1 61C62A8F
P 700 900
F 0 "#PWR?" H 700 750 50  0001 C CNN
F 1 "VCC" H 715 1073 50  0000 C CNN
F 2 "" H 700 900 50  0001 C CNN
F 3 "" H 700 900 50  0001 C CNN
	1    700  900 
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky_Small D?
U 1 1 61C64008
P 900 900
F 0 "D?" H 900 1107 50  0000 C CNN
F 1 "B5819W" H 900 1016 50  0000 C CNN
F 2 "" V 900 900 50  0001 C CNN
F 3 "~" V 900 900 50  0001 C CNN
	1    900  900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	700  900  800  900 
$Comp
L Device:Fuse_Small F?
U 1 1 61C65D2C
P 1250 900
F 0 "F?" H 1250 1085 50  0000 C CNN
F 1 "500mA" H 1250 994 50  0000 C CNN
F 2 "" H 1250 900 50  0001 C CNN
F 3 "~" H 1250 900 50  0001 C CNN
	1    1250 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 900  1000 900 
$Comp
L Device:Ferrite_Bead_Small FB?
U 1 1 61C66BDB
P 1600 900
F 0 "FB?" V 1363 900 50  0000 C CNN
F 1 "500 @ 100MHz" V 1454 900 50  0000 C CNN
F 2 "" V 1530 900 50  0001 C CNN
F 3 "~" H 1600 900 50  0001 C CNN
	1    1600 900 
	0    1    1    0   
$EndComp
Wire Wire Line
	1350 900  1500 900 
$Comp
L power:GND #PWR?
U 1 1 61C68EE9
P 2200 1250
F 0 "#PWR?" H 2200 1000 50  0001 C CNN
F 1 "GND" H 2205 1077 50  0000 C CNN
F 2 "" H 2200 1250 50  0001 C CNN
F 3 "" H 2200 1250 50  0001 C CNN
	1    2200 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61C6A631
P 1700 1000
F 0 "C?" H 1792 1046 50  0000 L CNN
F 1 "20uF" H 1792 955 50  0000 L CNN
F 2 "" H 1700 1000 50  0001 C CNN
F 3 "~" H 1700 1000 50  0001 C CNN
	1    1700 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61C6AB08
P 2650 1000
F 0 "C?" H 2742 1046 50  0000 L CNN
F 1 "20uF" H 2742 955 50  0000 L CNN
F 2 "" H 2650 1000 50  0001 C CNN
F 3 "~" H 2650 1000 50  0001 C CNN
	1    2650 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 900  2650 900 
$Comp
L power:+5V #PWR?
U 1 1 61C6BC98
P 2650 900
F 0 "#PWR?" H 2650 750 50  0001 C CNN
F 1 "+5V" H 2665 1073 50  0000 C CNN
F 2 "" H 2650 900 50  0001 C CNN
F 3 "" H 2650 900 50  0001 C CNN
	1    2650 900 
	1    0    0    -1  
$EndComp
Connection ~ 2650 900 
$Comp
L Regulator_Linear:AMS1117-5.0 U?
U 1 1 61C67798
P 2200 900
F 0 "U?" H 2200 1142 50  0000 C CNN
F 1 "AMS1117-5.0" H 2200 1051 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2200 1100 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 2300 650 50  0001 C CNN
	1    2200 900 
	1    0    0    -1  
$EndComp
Connection ~ 1700 900 
Wire Wire Line
	1700 900  1900 900 
Wire Wire Line
	2650 1100 2650 1200
Wire Wire Line
	2650 1200 2200 1200
Wire Wire Line
	1700 1100 1700 1200
Wire Wire Line
	1700 1200 2200 1200
Connection ~ 2200 1200
Wire Wire Line
	2200 1250 2200 1200
Wire Notes Line
	3000 1500 3000 600 
Wire Notes Line
	3000 600  600  600 
Wire Notes Line
	600  600  600  1500
Wire Notes Line
	600  1500 3000 1500
Text Notes 700  550  0    50   ~ 0
Input Voltage regulator
$Comp
L Connector:RJ45 J?
U 1 1 61C94C88
P 1950 2350
AR Path="/61C4DCDB/61C94C88" Ref="J?"  Part="1" 
AR Path="/61C94C88" Ref="J?"  Part="1" 
F 0 "J?" V 1961 2880 50  0000 L CNN
F 1 "RJ45" V 2052 2880 50  0000 L CNN
F 2 "" V 1950 2375 50  0001 C CNN
F 3 "~" V 1950 2375 50  0001 C CNN
	1    1950 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	1750 2800 1750 2750
Wire Wire Line
	1650 2750 1650 2800
Wire Wire Line
	1650 2800 1450 2800
$Comp
L MAX3107:MAX3107EAG+ U?
U 1 1 61C95CEA
P 2950 3300
AR Path="/61C4DCDB/61C95CEA" Ref="U?"  Part="1" 
AR Path="/61C95CEA" Ref="U?"  Part="1" 
F 0 "U?" H 3875 3587 60  0000 C CNN
F 1 "MAX3107EAG+" H 3875 3481 60  0000 C CNN
F 2 "21-0056C_24_MXM" H 3900 3490 60  0001 C CNN
F 3 "" H 2950 3300 60  0000 C CNN
	1    2950 3300
	1    0    0    -1  
$EndComp
$Comp
L arduino:Arduino_Leonardo_Shield XA?
U 1 1 61C9F8A2
P 7400 3700
F 0 "XA?" H 7400 5087 60  0000 C CNN
F 1 "Arduino_Leonardo_Shield" H 7400 4981 60  0000 C CNN
F 2 "" H 9200 7450 60  0001 C CNN
F 3 "https://store.arduino.cc/arduino-leonardo-with-headers" H 9200 7450 60  0001 C CNN
	1    7400 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61CAEC94
P 6000 4950
F 0 "#PWR?" H 6000 4700 50  0001 C CNN
F 1 "GND" H 6005 4777 50  0000 C CNN
F 2 "" H 6000 4950 50  0001 C CNN
F 3 "" H 6000 4950 50  0001 C CNN
	1    6000 4950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61CAF254
P 5950 4100
F 0 "#PWR?" H 5950 3950 50  0001 C CNN
F 1 "+5V" H 5965 4273 50  0000 C CNN
F 2 "" H 5950 4100 50  0001 C CNN
F 3 "" H 5950 4100 50  0001 C CNN
	1    5950 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 4450 6000 4450
Wire Wire Line
	6100 4350 6000 4350
Wire Wire Line
	6000 4350 6000 4450
Connection ~ 6000 4450
Wire Wire Line
	6000 4450 6000 4950
Wire Wire Line
	6100 4250 6000 4250
Wire Wire Line
	6000 4250 6000 4350
Connection ~ 6000 4350
Wire Wire Line
	5950 4100 5950 4650
Wire Wire Line
	5950 4650 6100 4650
NoConn ~ 6100 3350
NoConn ~ 6100 3450
NoConn ~ 6100 3550
NoConn ~ 6100 3650
NoConn ~ 6100 3750
NoConn ~ 6100 3850
NoConn ~ 6100 3950
NoConn ~ 6100 3050
Text GLabel 8700 3950 2    50   Input ~ 0
MISO
Text GLabel 8700 4050 2    50   Input ~ 0
MOSI
Text GLabel 8700 4150 2    50   Input ~ 0
SCK
NoConn ~ 1850 2750
NoConn ~ 1950 2750
NoConn ~ 2050 2750
NoConn ~ 2150 2750
NoConn ~ 2250 2750
NoConn ~ 2350 2750
Text GLabel 2950 3900 0    50   Input ~ 0
MISO
Text GLabel 2950 4200 0    50   Input ~ 0
MOSI
Text GLabel 2950 4000 0    50   Input ~ 0
SCK
NoConn ~ 8700 4750
Text GLabel 8700 4650 2    50   Input ~ 0
*rst
Text GLabel 2950 4400 0    50   Input ~ 0
*rst
Text GLabel 2950 4300 0    50   Input ~ 0
*UART-IRQ
Text GLabel 6100 2950 0    50   Input ~ 0
*UART-IRQ
$Comp
L power:+5V #PWR?
U 1 1 61CCACA3
P 2850 3200
F 0 "#PWR?" H 2850 3050 50  0001 C CNN
F 1 "+5V" H 2865 3373 50  0000 C CNN
F 2 "" H 2850 3200 50  0001 C CNN
F 3 "" H 2850 3200 50  0001 C CNN
	1    2850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3700 2850 3700
Wire Wire Line
	2850 3700 2850 3200
Text GLabel 1450 2800 0    50   Input ~ 0
HAN-RX-IN
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 61CD708C
P 950 3450
F 0 "J?" H 868 3767 50  0000 C CNN
F 1 "Conn_01x04" H 868 3676 50  0000 C CNN
F 2 "" H 950 3450 50  0001 C CNN
F 3 "~" H 950 3450 50  0001 C CNN
	1    950  3450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1450 3550 1450 3450
Wire Wire Line
	1450 3450 1150 3450
Wire Wire Line
	1550 3600 1550 3450
$Comp
L power:GND #PWR?
U 1 1 61CF801D
P 1550 3600
AR Path="/61C4DCDB/61CF801D" Ref="#PWR?"  Part="1" 
AR Path="/61CF801D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1550 3350 50  0001 C CNN
F 1 "GND" H 1555 3427 50  0000 C CNN
F 2 "" H 1550 3600 50  0001 C CNN
F 3 "" H 1550 3600 50  0001 C CNN
	1    1550 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 3450 2050 3550
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J?
U 1 1 61CD8C0A
P 1750 3350
F 0 "J?" H 1800 3567 50  0000 C CNN
F 1 "Conn_02x02" H 1800 3476 50  0000 C CNN
F 2 "" H 1750 3350 50  0001 C CNN
F 3 "~" H 1750 3350 50  0001 C CNN
	1    1750 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61C94C8E
P 1750 2800
AR Path="/61C4DCDB/61C94C8E" Ref="#PWR?"  Part="1" 
AR Path="/61C94C8E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1750 2550 50  0001 C CNN
F 1 "GND" H 1755 2627 50  0000 C CNN
F 2 "" H 1750 2800 50  0001 C CNN
F 3 "" H 1750 2800 50  0001 C CNN
	1    1750 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 3350 1550 3350
Wire Wire Line
	1450 3550 2050 3550
Text GLabel 1900 4150 2    50   Input ~ 0
EX-TX-IN
Text GLabel 1250 4250 0    50   Input ~ 0
EX-RX-IN
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J?
U 1 1 61D042E7
P 1450 4150
F 0 "J?" H 1500 4367 50  0000 C CNN
F 1 "Conn_02x02" H 1500 4276 50  0000 C CNN
F 2 "" H 1450 4150 50  0001 C CNN
F 3 "~" H 1450 4150 50  0001 C CNN
	1    1450 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 3900 1200 3550
Wire Wire Line
	1200 3550 1150 3550
Wire Wire Line
	1750 4150 1900 4150
$Comp
L power:+5V #PWR?
U 1 1 61D28A73
P 2100 3350
F 0 "#PWR?" H 2100 3200 50  0001 C CNN
F 1 "+5V" H 2115 3523 50  0000 C CNN
F 2 "" H 2100 3350 50  0001 C CNN
F 3 "" H 2100 3350 50  0001 C CNN
	1    2100 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 3350 2100 3350
Wire Wire Line
	1250 4150 1150 4150
Wire Wire Line
	1150 4150 1150 3650
Wire Wire Line
	1750 4250 1800 4250
Wire Wire Line
	1800 4250 1800 3900
Wire Wire Line
	1200 3900 1800 3900
Wire Wire Line
	2950 3800 2850 3800
Wire Wire Line
	2850 3800 2850 3700
Connection ~ 2850 3700
$Comp
L Device:C_Small C?
U 1 1 61D46098
P 2600 3700
F 0 "C?" H 2692 3746 50  0000 L CNN
F 1 "1uF" H 2692 3655 50  0000 L CNN
F 2 "" H 2600 3700 50  0001 C CNN
F 3 "~" H 2600 3700 50  0001 C CNN
	1    2600 3700
	1    0    0    -1  
$EndComp
NoConn ~ 3650 2650
$Comp
L power:GND #PWR?
U 1 1 61D475A9
P 2600 3800
F 0 "#PWR?" H 2600 3550 50  0001 C CNN
F 1 "GND" H 2605 3627 50  0000 C CNN
F 2 "" H 2600 3800 50  0001 C CNN
F 3 "" H 2600 3800 50  0001 C CNN
	1    2600 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3600 2600 3600
$Comp
L power:+5V #PWR?
U 1 1 61D4D460
P 5400 3200
F 0 "#PWR?" H 5400 3050 50  0001 C CNN
F 1 "+5V" H 5415 3373 50  0000 C CNN
F 2 "" H 5400 3200 50  0001 C CNN
F 3 "" H 5400 3200 50  0001 C CNN
	1    5400 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3400 5400 3400
Wire Wire Line
	5400 3400 5400 3200
Text GLabel 4800 3600 2    50   Input ~ 0
EX-RX-IN
Text GLabel 4800 3500 2    50   Input ~ 0
EX-TX-IN
$Comp
L Device:C_Small C?
U 1 1 61D58FF5
P 5400 3500
F 0 "C?" H 5492 3546 50  0000 L CNN
F 1 "0.1uF" H 5492 3455 50  0000 L CNN
F 2 "" H 5400 3500 50  0001 C CNN
F 3 "~" H 5400 3500 50  0001 C CNN
	1    5400 3500
	1    0    0    -1  
$EndComp
Connection ~ 5400 3400
$Comp
L power:GND #PWR?
U 1 1 61D5AA3E
P 5400 3600
F 0 "#PWR?" H 5400 3350 50  0001 C CNN
F 1 "GND" H 5405 3427 50  0000 C CNN
F 2 "" H 5400 3600 50  0001 C CNN
F 3 "" H 5400 3600 50  0001 C CNN
	1    5400 3600
	1    0    0    -1  
$EndComp
NoConn ~ 4800 3300
NoConn ~ 2950 3300
Text GLabel 2950 4100 0    50   Input ~ 0
*UART-CS
$EndSCHEMATC
