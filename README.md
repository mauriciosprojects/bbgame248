# BBGame248

BBGame248 is handheld game device that consists of an Arduino Nano processor board, an LED matrix display, four input buttons and a piezoelectric speaker. This device can be programmed using the Arduino IDE. This GitHub repository contains the BBGame248Lib library that can be used for programming this device, and it also contains a Space Invaders-like game. [Click here](https://vimeo.com/450081187) for a short demonstration video. 

![BBGame248 build diagram](https://github.com/mtejada11/BBGame248/blob/master/Build/BBGame248.png?raw=true)
*Diagram of BBGame248 assembled on breadboard*

# BBGame248 Arduino Library

The [BBGame248Lib.zip](https://github.com/mtejada11/BBGame248/blob/master/Lib/BBGame248Lib.zip?raw=true) library for Arduino included in this repository is a library that allows the BBGame248 device to be programmed with higher level methods/functions than the Arduino `digitalWrite` and `digitalRead` functions. See the page [BBGame248Lib Library Reference](https://github.com/mtejada11/BBGame248/wiki/BBGame248Lib-Library-Reference) page for more information on this library. 

# BBGame248 Coding Exercises

This repository contains several coding exercises. Exercises Ex01 to Ex07 are exercises than can be done with a partially assembled LED matrix of 8x8 pixels. The remaining exercises require the fully assembled BBGame248 circuit with and LED matrix of 24x8 pixels.

| Number | Description |
| --- | --- |
|Ex01  	|[Blink](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex01_Blink/Ex01_Blink.ino) |
|Ex02  	|[Sequence](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex02_Sequence/Ex02_Sequence.ino) |
|Ex02b 	|[Sequence with Button](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex02b_Sequence_with_Button/Ex02b_Sequence_with_Button.ino) |
|Ex03  	|[Text Input Output](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex03_Text_Input_Output/Ex03_Text_Input_Output.ino) |
|Ex03b 	|[Text Input Output LED Control](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex03b_Text_Input_Output_LED_Control/Ex03b_Text_Input_Output_LED_Control.ino) |
|Ex05  	|[Matrix Test](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex05_Matrix_Test/Ex05_Matrix_Test.ino) |
|Ex06  	|[Numeric Counter](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex06_Numeric_Counter/Ex06_Numeric_Counter.ino) |
|Ex07  	|[Set Pixels](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex07_Set_Pixels/Ex07_Set_Pixels.ino) |
|Ex07b 	|[Set Pixels with Photoresistor](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex07b_Set_Pixels_with_Photoresistor/Ex07b_Set_Pixels_with_Photoresistor.ino) |
|Ex08  	|[Scrolling Text](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex08_Scrolling_Text/Ex08_Scrolling_Text.ino) |
|Ex09  	|[Thousand Counter](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex09_Thousand_Counter/Ex09_Thousand_Counter.ino) |
|Ex09b 	|[Thousand Counter with Button](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex09b_Thousand_Counter_with_Button/Ex09b_Thousand_Counter_with_Button.ino) |
|Ex10a 	|[Game Elements (Ship)](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex10a_Game_Elements/Ex10a_Game_Elements.ino) |
|Ex10b 	|[Game Elements (Laser)](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex10b_Game_Elements/Ex10b_Game_Elements.ino) |
|Ex11b 	|[Light Meter Bar Graph](https://github.com/mtejada11/BBGame248/blob/master/Exercises/Ex11b_Light_Meter_Bar_Graph/Ex11b_Light_Meter_Bar_Graph.ino) |
