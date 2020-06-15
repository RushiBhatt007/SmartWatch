# SmartWatch

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/RushiBhatt007/SmartWatch/issues)

## Motivation

According to the 2011 census in India, about 5.1 million people suffered from some sort of visual impairment, the population of deaf people was estimated to be around 5 million and for dumb people it was estimated around 1.9 million people. Sensory impairment is the common term used to describe blindness, deafness, visual impairment, hearing impairment and in some cases combination of more than one of these. People suffering from sensory impairment face a lot of difficulties in their day to day activities. 

One of the most basic and key challenges faced by people with sensory impairment is the ability to tell time. Knowing what time it is a very basic human need and is a crucial parameter for almost every activity. Normally blind people have to rely on others to tell time which can prove to be inconvenient at times. Similarly people with other disabilities also find it difficult to tell time at times.

The motivation behind this project is based on two underlying issues. Firstly the commercial solutions present to address this issue target very specific demographics such as the blind and these products cannot be used by everyone who are experiencing sensory loss. The second issue is that these products are sold at exorbitant prices such as $300 which roughly is equivalent to Rs.23,000 which cannot be afforded by the average India who has an income of just Rs.11,254. Our project aims to design a watch that can be used by everyone facing sensory loss or not and at the same time be affordable for the average Indian.

## Proposed Model

The proposed model put forward by this project is aimed at being affordable for the average Indian household whilst catering to everyone who are experiencing any form of sensory loss. The watch has incorporated three approaches to indicate the time which are listed below:
*	A Speaker Unit which is used by the Text To Speech (TTS) system
* A Display Unit used to indicate time on its screen. 
* A Vibration Unit used by the Haptic feedback system.

While this outline gives the basic rubric of the system, some relevant points to be made such as the output from the speaker unit is in English, the time indicated by the display unit will be in 12 hour format. Furthermore the haptic feedback system first reads out the time before moving onto the seconds, all of this has been made possible via a microcontroller and switches in a move to increase the watches functionality and make it more useful for the user. 

One such functionality is the incorporation of a bluetooth module into the system thereby allowing the watch to be linked with the user’s phone. An android app has also been developed which provides additional features as listed below:
* Alarms 
* SOS Notifications (To 4 contacts via SMS and Email)
* Find My Watch
* Altering the volume functionality
* Altering the vibrational frequency  
* Customising the vibrational encoding scheme

Alarms can be set using the app which are then transferred to the watch which gets triggered when the current time matches the time on the alarm, SOS notifications can be used by the user when they feel are in danger, following the activation of the SOS feature the current LIVE location of the user is sent via an email to the user’s emergency contacts. The find my watch service allows the user to locate their watch as when triggered the watch will start to vibrate and emit audio.  The volume and the vibrational frequency can be changed to the user’s likings and using the app the user can modify the vibrational encoding scheme used by the watch, the vibrational encoding schemes have been developed with a focus on a small learning curve. 



## Top-down Approach
![alt text](https://github.com/RushiBhatt007/SmartWatch/blob/master/Readme%20Resources/Top-down%20approach.jpg?raw=true)
 
  ## Built With
 ### Software Platforms
 * [Android Studio](https://developer.android.com/studio) - Android Studio IDE used for building the android app
 * [Arduino IDE](https://www.arduino.cc/en/main/software/) - Arduino IDE used for programming the firmware
 * [Google Firebase](https://firebase.google.com/) - Firebase Realtime database is used
 ### Hardware components
 * Arduino UNO
 * Bluetooth Serial (HC-05)
 * OLED Module (GMS096A)
 * RTC Module (DS 3231)
 * Audio Amplifier (LM386)
 * Vibration Motor (Coin Vibration Motor)
 * 8 Ohm Speaker
 * Push Buttons X 4
 
 ## Block Diagram
![alt text](https://github.com/RushiBhatt007/SmartWatch/blob/master/Readme%20Resources/Block%20diagram.jpg?raw=true)
 
 ## Hardware Connections
 * 0 (Rx) - Tx of Bluetooth Module (HC05)
 * 1 (Tx) - Rx of Bluetooth Module (HC05)
 * ~3 (pwm) - Input of Audio Amplifier (LM386)
 * 4 - SOS Trigger Button
 * 5 - Screen Browse Button
 * 6 - Vibrate Time Button
 * 7 - Speak Time Button
 * ~9 (pwm) - Vibration Motor (Coin Vibration Motor) 
 * A4 - SCK of OLED Module (DS 3231) & SCL of RTC Module (DS 3231)
 * A5 - SDA of OLED Module (DS 3231) & SDA of RTC Module (DS 3231)
 
 Note: All the modules have been powered by +5V from Arduino Uno. However, only Audio Amplifier's (DS 3231) VDD is +12 V from an external power supply to get sufficient volume from the 8 Ohm Speaker.
 
 ## Hardware Implementation
![alt text](https://github.com/RushiBhatt007/SmartWatch/blob/master/Readme%20Resources/labelled%20hardware%20implementation.jpg?raw=true)

## Android App Screen Layout
![alt text](https://github.com/RushiBhatt007/SmartWatch/blob/master/Readme%20Resources/Android%20app%20screens.jpg?raw=true)
![alt text](https://github.com/RushiBhatt007/SmartWatch/blob/master/Readme%20Resources/Android%20SV.jpg?raw=true)
![alt text](https://github.com/RushiBhatt007/SmartWatch/blob/master/Readme%20Resources/Android%20SOS%20and%20FMW.jpg?raw=true)

## OLED Screen Layout
![alt text](https://github.com/RushiBhatt007/SmartWatch/blob/master/Readme%20Resources/OLED%20screens.jpg?raw=true)
 
 ## Authors
 * Rushi Bhatt
 * Siddharth Panikath
 * Vasu Eranki
 * Yash Gohil
 
 See also the list of [contributors](https://github.com/RushiBhatt007/SmartWatch/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details

## Acknowledgments
* [Prof. A. H. Lalluwadia](http://www.svnit.ac.in/web/department/electronics/faculty_eced.php) (Associate Professor, S. V. National Institute of Technology, Surat, Gujarat, India)
* [Dr. P. J. Engineer](http://svnit.ac.in/Data/facup/pjengineer/index.html) (Assistant Professor, S. V. National Institute of Technology, Surat, Gujarat, India)
