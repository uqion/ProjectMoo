This project aimed to model an internet of things (IoT) inspired cow feed monitoring system on an Arduino Mega2560 microcontroller board using an Adafruit PN532 NFC/RFID controller shield,13.56MHz MiFare Classic tags, anArduino ethernet shield, a HX711 Sparkfun amplifier, a 1 KG load cell, and a 3D-printed apparatus. Digital and analog readings from nearfield communication (NFC) and the load cell respectively are processed and output in real-time to both the Arduino IDE serial monitor and asimple webpage hosted on the Arduino ethernet server. Basic object-oriented principles formed the skeleton of the program, with all relevant functions falling under a ‘Cow’ class and relevant attributes belonging to ‘Cow’ objects.A standard C++ library was used to ensure the full gamut of C++ OOC techniques was available to us; those used for interfacing with sensors and shields, and the Timer1 library were also used in implementation.

   ![image](https://user-images.githubusercontent.com/31422704/50576255-aedc4b00-0dc2-11e9-8267-390979273aa4.png)


Final project for CPSC 1490: Applications of Microcontrollers

