# HackWestTX2025

## Inspiration
Our relatives work in the agricultural sector. They face the challenges of not receiving accurate and affordable weather statistics, as the weather predictions released by various stations are urban-centred and not rural-focused. This inspired us to build a project that is localised, affordable and accurate in nature.

## What it does
Our project senses, analyses and then reports the temperature, altitude, pressure, humidity, CO2 levels and TVOC(Total Volatile Organic Compounds) accurately. We plan to make it 100% solar-powered. The system displays all the values on the mini OLED display. It also uploads the data to a ThingSpeak server that can be accessed by anyone from anywhere. The values from Thingspeak are then read by our demonstrational HTML website that displays them in a more accessible manner.

## How we built it
We have used C++ for backend and HTML for frontend programming. Our project is based on the ESP8266. The sensors we currently have on board are CCS811(CO2 and TVOC level), MPL3115A2(temperature, pressure and altitude) and DHT11(humidity). As we lacked the resources to make it solar-powered, we are just powering it through a USB cable for now. In our schematic, we have included the circuit to make it completely solar powered.

## Challenges we ran into
We had trouble with the CCS811 and the DHT11 sensors. We also brought our own microcontroller, but did not bring a programming cable for it.

## Accomplishments that we're proud of
All of us were first-timers, and we are proud to have completed the project successfully. 

## What we learned
We understood and implemented Visual Studio Code, we learnt to use APIs, we dealt with frontend and backend programming, and also learnt to use git. We eradicated our fear of trying something we have little knowledge about.

## What's next for SenseStack
This project is not limited to farmers. It can be used by hikers and climbers, too. Any other field can modify it according to their requirements by simply adding or removing the sensors they need. We plan to make it modular just for that purpose. Once that is done, we can add any sensor we want and update the firmware to display the newer values.
