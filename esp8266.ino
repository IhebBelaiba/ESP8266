#include <ESP8266WebServer.h> 
#include <EEPROM.h>

ESP8266WebServer server(80);
 
const char* ssid = "HomeSmartHome";
const char* password = "12345678";

uint8_t newMACAddress[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

#define LED 2
int statusLED = LOW; // variable for LED status

// -----------------------------------------------------------------------------------------------------------------
const String HtmlHtml = 
"<html><head>"
"<meta name=\"viewport\" content=\"widt h=device-width, initial-scale=1\" /></head>";
const String HtmlTitle = 
"<h1>LED Control</h1><br/>\n";
const String HtmlLedStateLow = "<big>LED is now <b>OFF</b></big><br/><br/>\n";
const String HtmlLedStateHigh = "<big>LED is now <b>ON</b></big><br/><br/>\n";
const String HtmlButtons = 
"<a href='LEDOn'><button style='background-color:green;color:white;width:20%;height:5%;'>ON</button></a>&#09;"
"<a href='LEDOff'><button style='background-color:red;color:white;width:20%;height:5%;'>OFF</button></a><br/><br/>"
"<a href='WIFI'><button style='background-color:gray;color:white;width:20%;height:5%;'>Wi-Fi</button></a>&#09;"
"<a href='MAC'><button style='background-color:gray;color:white;width:20%;height:5%;'>MAC</button></a><br/>";
const String HtmlHtmlClose = 
"</html>";

const String HtmlWTitle = 
"<h1>Connect to network</h1><br/>\n";
const String HtmlForm1 =
"<form method='POST' action='/NEWMAC'>"
"<label for='ssid'>Wi-Fi:</label><br/><select id='ssid' name='ssid'>";
const String HtmlForm2 =
"</select><br><br><label for='pass'>Password:</label><br/>"
"<input id='pass' name='pass' type='password' length=64>"
"<br><br><input type='submit' value='Connect'></form>";

const String MACTitle = 
"<h1>Customize MAC address</h1><br/>\n";
const String MACForm =
"<form method='POST' action='/NEWMAC'>"
"<input id='mac1' name='mac1' pattern='[a-fA-F0-9]+' minlength='2' maxlength='2' style='width:4ch;'/>:"
"<input id='mac2' name='mac2' pattern='[a-fA-F0-9]+' minlength='2' maxlength='2' style='width:4ch;'/>:"
"<input id='mac3' name='mac3' pattern='[a-fA-F0-9]+' minlength='2' maxlength='2' style='width:4ch;'/>:"
"<input id='mac4' name='mac4' pattern='[a-fA-F0-9]+' minlength='2' maxlength='2' style='width:4ch;'/>:"
"<input id='mac5' name='mac5' pattern='[a-fA-F0-9]+' minlength='2' maxlength='2' style='width:4ch;'/>:"
"<input id='mac6' name='mac6' pattern='[a-fA-F0-9]+' minlength='2' maxlength='2' style='width:4ch;'/>"
"<br><br><input type='submit' value='Connect'></form>";
// -----------------------------------------------------------------------------------------------------------------

void handleLedOn() {
  statusLED = HIGH;
  digitalWrite(LED, !statusLED);
  response();
}

void handleLedOff() {
  statusLED = LOW;
  digitalWrite(LED, !statusLED);
  response();
}

void handleNEWWIFI() 
{
  if(!server.hasArg("ssid") || !server.hasArg("pass") || server.arg("ssid") == NULL)
  {
    server.send(400, "text/plain", "400: Invalid Request");
    return;
  }
  String a_ssid = server.arg("ssid");
  String a_pass = server.arg("pass");
  Serial.println(a_ssid);
  Serial.println(a_pass);
  if (a_ssid.length() > 0) 
  {
    for (int i = 0; i < 96; ++i) EEPROM.write(i, 0);
    for (int i = 0; i < a_ssid.length(); i++) EEPROM.write(i, a_ssid[i]);
    if(a_pass.length() > 0)
    {
      for (int i = 0; i < a_pass.length(); i++) EEPROM.write(32+i, a_pass[i]);
    }
    EEPROM.commit();
    ESP.restart();
  }
  else 
  {
    server.send(400, "text/plain", "400: Invalid Request");
  }
}

void handleWIFI(){
  String htmlRes = HtmlHtml + HtmlWTitle + HtmlForm1;
  delay(100);
  int n = WiFi.scanNetworks();
  String SSIDS[32];
  int SSID_count = 0;
  for (int i = 0; i < n; i++)
  {
    boolean found = false;
    for (int j = 0; j < SSID_count; j++)
    {
      if (SSIDS[j].equals(WiFi.SSID(i))) 
      {
        found = true;
      }
    }
    if(!found)
    {
      SSIDS[SSID_count] = WiFi.SSID(i);
      SSID_count++;
    }
  }
  for (int i = 0; i < SSID_count; i++)
  {
    htmlRes += "<option value='";
    htmlRes += SSIDS[i];
    htmlRes += "'>";
    htmlRes += SSIDS[i];
    htmlRes += "</option>";
  }
  htmlRes += HtmlForm2;
  htmlRes += HtmlHtmlClose;
  server.send(200, "text/html", htmlRes);
}

void handleMAC(){
  String htmlRes = HtmlHtml + MACTitle + MACForm;
  htmlRes += HtmlHtmlClose;
  server.send(200, "text/html", htmlRes);
}

void handleNEWMAC() 
{
  if( !server.hasArg("mac1") || server.arg("mac1") == NULL ||
      !server.hasArg("mac2") || server.arg("mac2") == NULL || 
      !server.hasArg("mac3") || server.arg("mac3") == NULL || 
      !server.hasArg("mac4") || server.arg("mac4") == NULL || 
      !server.hasArg("mac5") || server.arg("mac5") == NULL || 
      !server.hasArg("mac6") || server.arg("mac6") == NULL )
  {
    server.send(400, "text/plain", "400: Invalid Request");
    return;
  }
  String MACstr[6];
  char Str[] = "mac0";
  for (int i = 0; i < 6; ++i) 
  {    
    Str[3] = (i+49);
    MACstr[i] = server.arg(Str);
    MACstr[i].toUpperCase();
    byte tens = (MACstr[i][0] < '9') ? MACstr[i][0] - '0' : MACstr[i][0] - '7';
    byte ones = (MACstr[i][1] < '9') ? MACstr[i][1] - '0' : MACstr[i][1] - '7';
    byte number = (16 * tens) + ones;
    newMACAddress[i] = number;
    if(number > 255)
    {
      server.send(400, "text/plain", "400: Invalid Request");
      return;
    }
  }  
  for (int i = 96; i < 102; ++i) EEPROM.write(i, 0);
  for (int i = 0; i < 6; i++) EEPROM.write((i+96), newMACAddress[i]);
  EEPROM.commit();
  ESP.restart();
}


void response(){
  String htmlRes = HtmlHtml + HtmlTitle;
  if(statusLED) htmlRes += HtmlLedStateHigh;
  else htmlRes += HtmlLedStateLow;
  htmlRes += HtmlButtons;
  htmlRes += HtmlHtmlClose;
  server.send(200, "text/html", htmlRes);
}

bool isWifiOn()
{
  int c = 0;
  while (c < 15) 
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("\n-> Wifi Connection Established!");
      return true;
    }
    delay(1000);
    c++;
  }
  Serial.print("\n-> Connection timed out!");
  return false;
}
 
void setup() 
{
  delay(1000); 
  Serial.begin(115200);
  EEPROM.begin(512);
  String saved_ssid = "";
  String saved_pass = "";
  for (int i = 0; i < 32; i++) saved_ssid += char(EEPROM.read(i));
  for (int i = 32; i < 96; i++) saved_pass += char(EEPROM.read(i));
  
  WiFi.mode(WIFI_STA);
  for (int i = 0; i < 6; ++i) 
  { 
    newMACAddress[i] = EEPROM.read(i+96);
    Serial.print(newMACAddress[i]); 
    Serial.print(':'); 
  }
  
  wifi_set_macaddr(STATION_IF, &newMACAddress[0]);
  Serial.print("\n-> Custom MAC Address:  ");
  Serial.print(WiFi.macAddress());
  
  Serial.print("\n-> Connecting to: ");
  Serial.print(saved_ssid.c_str());
  Serial.print("; Password: ");
  Serial.print(saved_pass.c_str());
  WiFi.begin(saved_ssid.c_str(), saved_pass.c_str());
  if(!isWifiOn())
  {
    WiFi.mode(WIFI_OFF);
    Serial.print("\n-> Establishing Access Point");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    IPAddress apip = WiFi.softAPIP();
    Serial.print("\n-> Access Point Established!");
  }
  else Serial.println(WiFi.localIP());
  server.on("/", response); 
  server.on("/LEDOn", handleLedOn);
  server.on("/LEDOff", handleLedOff);
  server.on("/WIFI", handleWIFI);
  server.on("/MAC", handleMAC);
  server.on("/NEWWIFI", HTTP_POST, handleNEWWIFI);
  server.on("/NEWMAC", HTTP_POST, handleNEWMAC);
  server.begin();
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, !statusLED);
}
 
void loop() 
{
  server.handleClient();
}
