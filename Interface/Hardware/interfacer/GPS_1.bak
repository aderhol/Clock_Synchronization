EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:auxiliary
LIBS:interfacer-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 9
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
L L86 U4
U 1 1 59CE141D
P 5150 3900
F 0 "U4" H 5150 4400 60  0000 C CNN
F 1 "L86" H 5150 3350 60  0000 C CNN
F 2 "auxiliary:L86" H 3550 4650 60  0001 C CNN
F 3 "" H 3550 4650 60  0001 C CNN
	1    5150 3900
	1    0    0    -1  
$EndComp
NoConn ~ 4500 4150
NoConn ~ 4500 3550
NoConn ~ 4500 3700
$Comp
L C C15
U 1 1 59CE1620
P 6750 3850
F 0 "C15" H 6775 3950 50  0000 L CNN
F 1 "10u" H 6775 3750 50  0000 L CNN
F 2 "auxiliary:0805_2012" H 6788 3700 50  0001 C CNN
F 3 "" H 6750 3850 50  0001 C CNN
	1    6750 3850
	1    0    0    -1  
$EndComp
$Comp
L C C14
U 1 1 59CE165B
P 6500 3850
F 0 "C14" H 6525 3950 50  0000 L CNN
F 1 "1u" H 6525 3750 50  0000 L CNN
F 2 "auxiliary:0805_2012" H 6538 3700 50  0001 C CNN
F 3 "" H 6500 3850 50  0001 C CNN
	1    6500 3850
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 59CE167E
P 6200 3850
F 0 "C13" H 6225 3950 50  0000 L CNN
F 1 "0.1u" H 6225 3750 50  0000 L CNN
F 2 "auxiliary:0805_2012" H 6238 3700 50  0001 C CNN
F 3 "" H 6200 3850 50  0001 C CNN
	1    6200 3850
	1    0    0    -1  
$EndComp
Text HLabel 4250 4000 0    60   Input ~ 0
Reset
Text HLabel 6050 4150 2    60   Output ~ 0
UART_Tx
Text HLabel 6050 4300 2    60   Input ~ 0
UART_Rx
Text HLabel 6050 3550 2    60   Output ~ 0
PPS
$Comp
L GND #PWR046
U 1 1 59CE1EBA
P 7550 4300
F 0 "#PWR046" H 7550 4050 50  0001 C CNN
F 1 "GND" H 7550 4150 50  0000 C CNN
F 2 "" H 7550 4300 50  0001 C CNN
F 3 "" H 7550 4300 50  0001 C CNN
	1    7550 4300
	1    0    0    -1  
$EndComp
Text HLabel 7550 3450 2    60   UnSpc ~ 0
3V3
$Comp
L GND #PWR047
U 1 1 59CE22D8
P 4250 4350
F 0 "#PWR047" H 4250 4100 50  0001 C CNN
F 1 "GND" H 4250 4200 50  0000 C CNN
F 2 "" H 4250 4350 50  0001 C CNN
F 3 "" H 4250 4350 50  0001 C CNN
	1    4250 4350
	1    0    0    -1  
$EndComp
$Comp
L R R14
U 1 1 59D3C726
P 7300 4050
F 0 "R14" V 7380 4050 50  0000 C CNN
F 1 "1k5" V 7300 4050 50  0000 C CNN
F 2 "auxiliary:0805_2012" V 7230 4050 50  0001 C CNN
F 3 "" H 7300 4050 50  0001 C CNN
F 4 "2012" V 7300 4050 60  0001 C CNN "Package"
	1    7300 4050
	1    0    0    -1  
$EndComp
$Comp
L LED_ALT D15
U 1 1 59D3C731
P 7300 3650
F 0 "D15" H 7300 3750 50  0000 C CNN
F 1 "GPS1 3V3" H 7300 3550 50  0000 C CNN
F 2 "auxiliary:0805_2012" H 7300 3650 50  0001 C CNN
F 3 "http://lomex.hu/pdf/(kin)_kp-2012surc.pdf" H 7300 3650 50  0001 C CNN
F 4 "2012" H 7300 3650 60  0001 C CNN "Package"
F 5 "KP-2012SURC" H 7300 3650 60  0001 C CNN "part#"
F 6 "95-01-14" H 7300 3650 60  0001 C CNN "Lomex"
F 7 "red" H 7300 3650 60  0001 C CNN "Color"
	1    7300 3650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5800 3700 6050 3700
Connection ~ 7300 4250
Connection ~ 7300 3450
Wire Wire Line
	7000 4050 7000 4250
Wire Wire Line
	7000 4250 7550 4250
Wire Wire Line
	7000 3650 7000 3450
Wire Wire Line
	7000 3450 7550 3450
Wire Wire Line
	7300 3500 7300 3450
Wire Wire Line
	7300 4250 7300 4200
Wire Wire Line
	7300 3800 7300 3900
Wire Wire Line
	4250 4300 4250 4350
Wire Wire Line
	4500 4300 4250 4300
Connection ~ 6750 4050
Wire Wire Line
	6750 4000 6750 4050
Connection ~ 6500 4050
Wire Wire Line
	6500 4000 6500 4050
Connection ~ 6200 4050
Wire Wire Line
	6200 4000 6200 4050
Wire Wire Line
	7550 4250 7550 4300
Wire Wire Line
	6050 4050 7000 4050
Wire Wire Line
	6050 4000 6050 4050
Wire Wire Line
	5800 4000 6050 4000
Connection ~ 6750 3650
Connection ~ 6500 3650
Wire Wire Line
	6750 3650 6750 3700
Connection ~ 6200 3650
Wire Wire Line
	6500 3650 6500 3700
Wire Wire Line
	6200 3650 6200 3700
Wire Wire Line
	5800 3550 6050 3550
Wire Wire Line
	5800 4300 6050 4300
Wire Wire Line
	5800 4150 6050 4150
Wire Wire Line
	4500 4000 4250 4000
Wire Wire Line
	6050 3700 6050 3650
Wire Wire Line
	6050 3650 7000 3650
Wire Wire Line
	5800 3850 5900 3850
Wire Wire Line
	5900 3850 5900 3700
Connection ~ 5900 3700
$EndSCHEMATC
