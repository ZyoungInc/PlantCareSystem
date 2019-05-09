#define DHTPIN D4
#define DHTTYPE DHT11
#define WATERPIN D3
#define STASSID "Allen's iPhone"
#define STAPSK  "allenlum"
const int ScreenRolls = 30;
const int LOGOlines = 12;
const char* ssid = STASSID;
const char* password = STAPSK;

String LOGO[] = {
  "  _____  _             _    _____                 ________",
  " |  __ \\| |           | |  / ____|               /  ____  \\",
  " | |__) | | __ _ _ __ | |_| |     __ _ _ __ ___ /  |  _ \\  \\",
  " |  ___/| |/ _` | '_ \\| __| |    / _` | '__/ _ \\   | |_) |  |",
  " | |    | | (_| | | | | |_| |___| (_| | | |  __/   |  _ <   |",
  " |_|    |_|\\__,_|_| |_|\\__|\\_____\\__,_|_|  \\___|\\  |_| \\_\\ /",
  "          ____   _____  __      _____   ___    _ \\________/",
  "         / __ \\ / ____| \\ \\    / / _ \\ / _ \\  | |",
  "        | |  | | (___    \\ \\  / / | | | (_) | | |__",
  "        | |  | |\\___ \\    \\ \\/ /| | | |\\__, | | '_ \\",
  "        | |__| |____) |    \\  / | |_| |  / /  | |_) |",
  "         \\____/|_____/      \\/   \\___(_)/_/   |_.__/"
};

//define sensor
WiFiServer server(80);
Relay Water(WATERPIN);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_seesaw SoilSensor;


float h, t, f, tempC, tempF;
uint16_t capread;
int targetlow, targethi, RTemplow, Humidlow, Humidhi, RTemphi, Soiltemplow, Soiltemphi, Operation;
String req;
int oldaction,intaction=0;

void SetSensorParameterVeg() {
  //Checking for Mositure in Soil
  targetlow = 350; //set target when need water
  targethi = 600; // set target when there is too much water
  // Checking for Soil temperature
  Soiltemplow = 65;
  Soiltemphi = 75;
  // Checking the Room temperature
  RTemplow = 70;
  RTemphi = 79;
  // Checking the Room Humidity
  Humidlow = 45;
  Humidhi = 55;
}
void SetSensorParameterFlo() {
  //Checking for Mositure in Soil
  targetlow = 650; //set target when need water
  targethi = 800; // set target when there is too much water
  // Checking for Soil temperature
  Soiltemplow = 62;
  Soiltemphi = 70;
  // Checking the Room temperature
  RTemplow = 68;
  RTemphi = 75;
  // Checking the Room Humidity
  Humidlow = 45;
  Humidhi = 55;
}
void SetSensorParameterHar() {
  //Checking for Mositure in Soil
  targetlow = 350; //set target when need water
  targethi = 600; // set target when there is too much water
  // Checking for Soil temperature
  Soiltemplow = 62;
  Soiltemphi = 70;
  // Checking the Room temperature
  RTemplow = 68;
  RTemphi = 75;
  // Checking the Room Humidity
  Humidlow = 35;
  Humidhi = 45;
}
void SetSensorParameterDry() {
  //Checking for Mositure in Soil
  targetlow = 350; //set target when need water
  targethi = 600; // set target when there is too much water
  // Checking for Soil temperature
  Soiltemplow = 65;
  Soiltemphi = 75;
  // Checking the Room temperature
  RTemplow = 70;
  RTemphi = 79;
  // Checking the Room Humidity
  Humidlow = 45;
  Humidhi = 55;
}
