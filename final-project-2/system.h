#include "DHT.h"
#include "Adafruit_seesaw.h"
#include "ESP8266WiFi.h"
#include "driver.h"
#include "config.h"
void RefreshSensor() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  //these data comes from DHT11
  tempC = SoilSensor.getTemp();
  capread = SoilSensor.touchRead(0);
  tempF = tempC * (9 / 5) + 32;
}
void DoAction() {
  switch (Operation) {
    case 0:
      break;
    case 1:
      SetSensorParameterVeg();
      break;
    case 2:
      SetSensorParameterFlo();
      break;
    case 3:
      SetSensorParameterHar();
      break;
    case 4:
      SetSensorParameterDry();
      break;
    case 5:
      Water.Off();
      break;
    case 6:
      Water.On();
      break;
    case 7:
      Water.Off();
      break;
    case 8:
      ESP.restart();
      break;
  }
}
void WebServer() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));
  // Read the first line of the request
  req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);
  if (req.indexOf(F("/gpio/0")) != -1) {
    intaction = 0;
  } else if (req.indexOf(F("/gpio/1")) != -1) {
    intaction = 1;
  } else if (req.indexOf(F("/gpio/2")) != -1) {
    intaction = 2;
  } else if (req.indexOf(F("/gpio/3")) != -1) {
    intaction = 3;
  } else if (req.indexOf(F("/gpio/4")) != -1) {
    intaction = 4;
  } else if (req.indexOf(F("/gpio/5")) != -1) {
    intaction = 5;
  } else if (req.indexOf(F("/gpio/6")) != -1) {
    intaction = 6;
  } else if (req.indexOf(F("/gpio/7")) != -1) {
    intaction = 7;
  } else if (req.indexOf(F("/gpio/8")) != -1) {
    intaction = 8;
  } else {
    Serial.println(F("invalid request"));
    intaction = 0;
  }
  if (oldaction == intaction) {
    Operation = 0;
  }
  else {
    Operation = intaction;
    oldaction = intaction;
  }
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet



  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<head>\r\n <meta http-equiv=\"refresh\" content=\"2\"<head>\r\nSoil Mositure: "));
  client.print(capread);
  client.print(F("; Soil Temperature:"));
  client.print(tempC);
  client.print(F("C ; "));
  client.print(tempF);
  client.print(F("F;"));
  client.print(F("<br><br>Room Humidity:"));
  client.print(h);
  client.print(F("% ; Room Temperature:"));
  client.print(t);
  client.print(F("C ; "));
  client.print(f);
  client.print(F("F;"));

  client.print(F("<br><br>Soil Moisture Range: "));
  client.print(targetlow);
  client.print(F("~"));
  client.print(targethi);
  client.print(F("<br><br>Room Temperature Range: "));
  client.print(RTemplow);
  client.print(F("~"));
  client.print(RTemphi);
  client.print(F(" F<br><br>Room Humidity Range: "));
  client.print(Humidlow);
  client.print(F("~"));
  client.print(Humidhi);
  client.print(F(" %<br><br>Soil Temperature Range: "));
  client.print(Soiltemplow);
  client.print(F("~"));
  client.print(Soiltemphi);
  client.print(F(" F<br><br>Operation is now:"));
  client.print(Operation);

  if (Operation < 7) {
    if (targethi < capread) {
      client.print(F("<br><br><p style=\"background-color:red\">Alert:Plant Drowning: "));
      client.print (capread);
      client.print (F("</p>"));
      Water.On();
    }
    else if (targetlow > capread) {
      client.print(F("<br><br><p style=\"background-color:red\">Adding:Water, soil dry"));
      client.print (F("</p>"));
      Water.Off();
    }

    if (RTemplow > f) {
      client.print(F("<br><br><p style=\"background-color:red\">Alert:Room Too COLD: "));
      client.print (f);
      client.print(F(" F</p>"));
    }
    else if ( RTemphi < f) {
      client.print(F("<br><br><p style=\"background-color:red\">Alert:Room Too HOT: "));
      client.print (f);
      client.print(F(" F</p>"));
    }
    if (Humidlow > h) {
      client.print(F("<br><br><p style=\"background-color:red\">Alert:Room Little Humidity: "));
      client.println (h);
      client.print (F("</p>"));

    }
    else if ( Humidhi < h) {
      client.print(F("<br><br><p style=\"background-color:red\">Alert:Room Too Humid: "));
      client.print (h);
      client.print (F("</p>"));
    }
    if (Soiltemphi < tempF) {
      client.print (F("<br><br><p style=\"background-color:red\">Alert:Soil is too Hot:"));
      client.print(tempF);
      client.print(F(" F</p>"));
    }
    else if (Soiltemplow > tempF) {
      client.print (F("<br><br><p style=\"background-color:red\">Alert:Soil is too Cold:"));
      client.print(tempF);
      client.print(F(" F</p>"));
    }
  }
  client.print(F("<br><br>Funcations : <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/0'>Auto</a> ; <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'>Vegetation</a> ; <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/2'>Flowering</a> ; <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/3'>Harvesting</a> ; <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/4'>Drying</a> ; <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/7'>Manual switch water</a> ; <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/8'>Rest</a> to reset</html>"));




  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}
void SetupDHT11Sensor() {
  Serial.println(F("Initializing DH11."));
  dht.begin();
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {

    Serial.println(F("Failed to read from DHT sensor!"));

    return;

  }


  Serial.print(F("Humidity: "));

  Serial.print(h);

  Serial.print(F("%  Temperature: "));

  Serial.print(t);

  Serial.print(F("°C "));

  Serial.print(f);

  float hif = dht.computeHeatIndex(f, h);

  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("°F  Heat index: "));

  Serial.print(hic);

  Serial.print(F("°C "));

  Serial.print(hif);

  Serial.println(F("°F"));
  Serial.println(F("initialization of DHT11 successfull."));
}
void SetupSoilSensor() {
  Serial.println("seesaw Soil Sensor starting!");

  if (!SoilSensor.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while (1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(SoilSensor.getVersion(), HEX);
  }
}
void SetupWifi() {
  Serial.println();
  Serial.print("Wifi Mode selected: STA. ");
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println(F("Web Server starting"));
  server.begin();
  Serial.println(F("Server started"));
}
void StartScreen() {
  for (int temptime = 0; temptime < LOGOlines; temptime++) {
    Serial.println(LOGO[temptime]);
  }
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  delay(2000);
}
void CleanScreen() {
  for (int temptime = 0; temptime < ScreenRolls; temptime++) {
    Serial.println("");
  }
}
