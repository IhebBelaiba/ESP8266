# ESP8266 Web Server (WiFi/AP)
ESP8266 Web Server (WiFi/AP)

<b>Operation:</b>
- The ship runs in AP mode by default.
- Create a webserver to controll the ESP8266.
- Use the ship to controll the built in LED.
- Save ssid and password to EEPROM.
- Customize MAC address in station mode and save to EEPROM.

![alt text](https://raw.githubusercontent.com/IhebBelaiba/ESP8266-WebServer/main/Screenshots/main.png)

<b>Usage (Using browser):</b>
1. Connect your computer/phone to the Access Point created by the ESP8266.
2. Use your web browser to access the Web Server on http://192.168.4.1/ (default IP address for ESP8266)
3. Click "Wi-Fi" button. Select the SSID and enter the password and hit "Connect".
4. The microchip will restart and try to connect to the Network you selected. If the connection attempt times out (15 seconds) the ESP8266 will run in Access Point mode again.
5. Use serial monitor on 115200 baud to read the new IP address given the ESP8266.

![alt text](https://raw.githubusercontent.com/IhebBelaiba/ESP8266-WebServer/main/Screenshots/WiFi.png)

<b>Notice:</b></br>
If you have a MAC filtering system on your router. You can customize your microship MAC address.
1. Use your web browser to access the Web Server on the default IP address or the new one from the serial monitor if your microship is already connected to a Network.
3. Click "MAC" button. Enter your custom MAC address in hexadecimal in the form 00:00:00:00:00:00 and hit "Set".
4. The microchip will restart and use the new MAC address to connect to the Network saved on the memory if it exists.

![alt text](https://raw.githubusercontent.com/IhebBelaiba/ESP8266-WebServer/main/Screenshots/MAC.png)
