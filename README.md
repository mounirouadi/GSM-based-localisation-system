# GSM-based-localisation-system
This project is an Arduino-based localization system that uses a Neoway GSM module and GPS module to determine the location of a device and send it via SMS using AT commands. The user can call the SIM card associated with the device to receive the location information as an SMS. The project can be customized by modifying the code.

# Hardware
* Arduino board (e.g. Uno, Mega, etc.)
* Neoway GSM module
* GPS module
* Jumper wires
* Breadboard or protoboard (optional)
* LiPO battery 
* charging module
# Software
* Arduino IDE (download at https://www.arduino.cc/en/Main/Software)
# Setup
* Connect the hardware according to the schematics in the schematics folder.
* Open the localization.ino file in the Arduino IDE.
* Modify the constants at the top of the file (e.g. GSM_RX_PIN, GSM_TX_PIN, GPS_RX_PIN, GPS_TX_PIN, etc.) to match the connections made in step 1.
* Upload the code to the Arduino board.
# Usage
Call the SIM card associated with the Neoway GSM module. The call will be automatically hung up, and the device's location will be determined using the GPS module.
The location information will be sent as an SMS to the caller.
# Customization
You can customize the behavior of the localization system by modifying the code in the localization.ino file. For example, you can change the format of the SMS message or add additional AT commands to the code.

Note: Make sure to consult the documentation for the Neoway GSM module and GPS module to learn about the available AT commands and how to use them.
