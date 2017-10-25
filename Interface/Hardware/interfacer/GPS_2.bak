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
Sheet 6 9
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
L Conn_02x04_Top_Bottom J13
U 1 1 59CD3636
P 6350 4250
F 0 "J13" H 6400 4450 50  0000 C CNN
F 1 "GPS2 conn" H 6400 3950 50  0000 C CNN
F 2 "auxiliary:Pin_Header_Straight_2x04_Pitch2.00mm_column-numbering" H 6350 4250 50  0001 C CNN
F 3 "https://lomex.hu/pdf/phds22.pdf" H 6350 4250 50  0001 C CNN
F 4 "43-10-39" H 6350 4250 60  0001 C CNN "Lomex"
F 5 "2mm" H 6350 4250 60  0001 C CNN "Spacing"
	1    6350 4250
	1    0    0    -1  
$EndComp
NoConn ~ 6650 4350
NoConn ~ 6150 4450
Text Label 6150 4150 2    60   ~ 0
ANT_PWR
Text Label 6150 4250 2    60   ~ 0
Vcc
Text Label 6150 4350 2    60   ~ 0
Tx
Text Label 6650 4150 0    60   ~ 0
Rx
Text Label 6650 4250 0    60   ~ 0
PPS
Text Label 6650 4450 0    60   ~ 0
GND
Text HLabel 7100 4250 2    60   Output ~ 0
PPS
Text HLabel 7100 4150 2    60   Input ~ 0
UART_Rx
Text HLabel 5800 4350 0    60   Output ~ 0
UART_Tx
$Comp
L GND #PWR043
U 1 1 59CD3EEC
P 3750 4900
F 0 "#PWR043" H 3750 4650 50  0001 C CNN
F 1 "GND" H 3750 4750 50  0000 C CNN
F 2 "" H 3750 4900 50  0001 C CNN
F 3 "" H 3750 4900 50  0001 C CNN
	1    3750 4900
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 59CD4063
P 5150 4450
F 0 "C10" H 5175 4550 50  0000 L CNN
F 1 "0.01u" H 5175 4350 50  0000 L CNN
F 2 "auxiliary:0805_2012" H 5188 4300 50  0001 C CNN
F 3 "" H 5150 4450 50  0001 C CNN
	1    5150 4450
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 59CD40EF
P 4800 4450
F 0 "C9" H 4825 4550 50  0000 L CNN
F 1 "0.1u" H 4825 4350 50  0000 L CNN
F 2 "auxiliary:0805_2012" H 4838 4300 50  0001 C CNN
F 3 "" H 4800 4450 50  0001 C CNN
	1    4800 4450
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 59CD415C
P 4500 4450
F 0 "C8" H 4525 4550 50  0000 L CNN
F 1 "1u" H 4525 4350 50  0000 L CNN
F 2 "auxiliary:0805_2012" H 4538 4300 50  0001 C CNN
F 3 "" H 4500 4450 50  0001 C CNN
	1    4500 4450
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NC_Dual JP7
U 1 1 59CD4A8A
P 5450 4000
F 0 "JP7" H 5500 3900 50  0000 L CNN
F 1 "GPS2 ant pwr" H 5450 4100 50  0000 C BNN
F 2 "auxiliary:3P_2.54_jumper" H 5450 4000 50  0001 C CNN
F 3 "" H 5450 4000 50  0001 C CNN
	1    5450 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 4150 7100 4150
Wire Wire Line
	5450 4150 6150 4150
Wire Wire Line
	4300 4250 6150 4250
Wire Wire Line
	6150 4350 5800 4350
Wire Wire Line
	6650 4450 7100 4450
Wire Wire Line
	6650 4250 7100 4250
Wire Wire Line
	7100 4450 7100 4650
Wire Wire Line
	4500 4300 4500 4250
Connection ~ 4500 4250
Wire Wire Line
	4800 4300 4800 4250
Connection ~ 4800 4250
Wire Wire Line
	5150 4000 5150 4300
Connection ~ 5150 4250
Wire Wire Line
	7100 4650 4300 4650
Wire Wire Line
	5150 4650 5150 4600
Wire Wire Line
	4500 4600 4500 4650
Connection ~ 4500 4650
Wire Wire Line
	4800 4600 4800 4650
Connection ~ 4800 4650
Connection ~ 5150 4650
Wire Wire Line
	3750 4850 3750 4900
Wire Wire Line
	5200 4000 5150 4000
Wire Wire Line
	5450 4100 5450 4150
$Comp
L C C12
U 1 1 59CD4F5C
P 5950 3750
F 0 "C12" H 5975 3850 50  0000 L CNN
F 1 "0.1u" H 5975 3650 50  0000 L CNN
F 2 "auxiliary:0805_2012" H 5988 3600 50  0001 C CNN
F 3 "" H 5950 3750 50  0001 C CNN
	1    5950 3750
	0    -1   -1   0   
$EndComp
$Comp
L C C11
U 1 1 59CD4F62
P 5950 3400
F 0 "C11" H 5975 3500 50  0000 L CNN
F 1 "1u" H 5975 3300 50  0000 L CNN
F 2 "auxiliary:0805_2012" H 5988 3250 50  0001 C CNN
F 3 "" H 5950 3400 50  0001 C CNN
	1    5950 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6150 3400 6150 3800
Wire Wire Line
	6150 3400 6100 3400
Wire Wire Line
	6100 3750 6150 3750
Connection ~ 6150 3750
Wire Wire Line
	5800 3400 5750 3400
Wire Wire Line
	5750 3250 5750 4000
Wire Wire Line
	5750 3750 5800 3750
$Comp
L GND #PWR044
U 1 1 59CD505A
P 6150 3800
F 0 "#PWR044" H 6150 3550 50  0001 C CNN
F 1 "GND" H 6150 3650 50  0000 C CNN
F 2 "" H 6150 3800 50  0001 C CNN
F 3 "" H 6150 3800 50  0001 C CNN
	1    6150 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4000 5700 4000
Connection ~ 5750 3750
$Comp
L +5V #PWR045
U 1 1 59CD50F4
P 5750 3250
F 0 "#PWR045" H 5750 3100 50  0001 C CNN
F 1 "+5V" H 5750 3390 50  0000 C CNN
F 2 "" H 5750 3250 50  0001 C CNN
F 3 "" H 5750 3250 50  0001 C CNN
	1    5750 3250
	1    0    0    -1  
$EndComp
Connection ~ 5750 3400
Text HLabel 3750 4050 0    60   UnSpc ~ 0
3V3
$Comp
L R R13
U 1 1 59D3CEFB
P 4000 4650
F 0 "R13" V 4080 4650 50  0000 C CNN
F 1 "1k5" V 4000 4650 50  0000 C CNN
F 2 "auxiliary:0805_2012" V 3930 4650 50  0001 C CNN
F 3 "" H 4000 4650 50  0001 C CNN
F 4 "2012" V 4000 4650 60  0001 C CNN "Package"
	1    4000 4650
	1    0    0    -1  
$EndComp
$Comp
L LED_ALT D14
U 1 1 59D3CF06
P 4000 4250
F 0 "D14" H 4000 4350 50  0000 C CNN
F 1 "GPS2 3V3" H 4000 4150 50  0000 C CNN
F 2 "auxiliary:0805_2012" H 4000 4250 50  0001 C CNN
F 3 "http://lomex.hu/pdf/(kin)_kp-2012surc.pdf" H 4000 4250 50  0001 C CNN
F 4 "2012" H 4000 4250 60  0001 C CNN "Package"
F 5 "KP-2012SURC" H 4000 4250 60  0001 C CNN "part#"
F 6 "95-01-14" H 4000 4250 60  0001 C CNN "Lomex"
F 7 "red" H 4000 4250 60  0001 C CNN "Color"
	1    4000 4250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4000 4400 4000 4500
Wire Wire Line
	4000 4850 4000 4800
Wire Wire Line
	4000 4100 4000 4050
Wire Wire Line
	3750 4050 4300 4050
Wire Wire Line
	4300 4050 4300 4250
Wire Wire Line
	3750 4850 4300 4850
Wire Wire Line
	4300 4850 4300 4650
Connection ~ 4000 4050
Connection ~ 4000 4850
$EndSCHEMATC
