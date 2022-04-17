/***************************************************
  This code is edited by Masud Rana for the project called
  IoT Attendance System
  Masud Rana
  KUET
  EEE,2K16

  **Added multiple google sheet authcode
****************************************************/

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <HTTPClient.h>
#include <string.h>
#include <WiFi.h>

// For SD card
#include "FS.h"
#include "SD.h"
#include "SPI.h"
String NAME;
int last_class_no = 0;
String SD_cardpath_C;
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  last_class_no = 0;
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
      last_class_no = last_class_no + 1;
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char * path, int c) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  String mms = "";
  while (file.available()) {
    //    char arr[15];
    //Serial.write(file.read());
    char mm = file.read();

    //    if(mm != '\n')
    mms = mms + mm;

    //    String mms= String(mm);

    //int sl=mms.length();

  }
  Serial.println(mms);
  file.close();

  int StringCount = 0;
  String rows[100];
  // Split the string into substrings
  while (mms.length() > 0)
  {
    int index = mms.indexOf('\n');
    if (index == -1) // No space found
    {
      rows[StringCount++] = mms;
      break;
    }
    else
    {
      rows[StringCount++] = mms.substring(0, index);
      mms = mms.substring(index + 1);
    }
  }

  String temps = "";
  String row = "";
  NAME = "Class-" + String(c) + ",";
  for (int i = 1; rows[i] != ""; i++) {
    row = rows[i];
    NAME = NAME + row + ",";
  }
  char flagg = NAME[NAME.length() - 2];
  Serial.print("Flag:"); Serial.println(flagg);
  if (flagg == '0') {
    sendData("NAME=" + NAME);
    appendFile(SD, path, "Done,1\n");
  }
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

//for AP mode
#include "EEPROM.h"
#include "HTML.h"
#include <WebServer.h>
WebServer server(80);
#include "WiFiManager.h"

#include <Wire.h>

//For Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//Rotary encoder
#define CLK    32
#define DT     33
#define SW     25
int counter = 0; int currentCLK; int previousCLK;
String encdir = "";
void get_rotation();
///////////
void choosesub();
void enroll();
void GetFingerprintID();

const char* ID;
uint8_t id;
int pass1 = 1;

// These will come from EEPROM
const char* ssid;
const char* pass;
String GOOGLE_SCRIPT_ID; // Replace by your GAS service id
String GS_ID_Ch1103 = "AKfycbyP4uilvSbROeXs44C4ly3E4hDpUOxETqZhcd_WJeYxU0iqna-AteAQi5-MOtpzBzZJ";
String GS_ID_EE1103 = "AKfycbzT6bifaVThFx8raYXgzzoRspCtNtTL4CpLZEEJVyJZPnhogx2TZfxWawgwpNj1VgVWkg";
String GS_ID_Hum1103 = "AKfycbx9RwdBlU4lAoRTWGbU9k5zGeqcLzOGRBqmnYzWhMbGtuuGAKEoQfZ8o8WIHwHSmthk";
String GS_ID_Math1103 = "AKfycbz4zHUloYXoGWdRB5AX5nHuhAywnE2r2L2pFmSXGpMxqm39m-KqJOVqpgN_tNF559n_";
String GS_ID_Ph1103 = "AKfycbztriZA555IPoZZNnPNMPUOLBgD0S-QZ18ZP14brSC6JzfW5FijWmXvdcFtgxk2l0HX";

const int sendInterval = 996 * 5;
// in millis, 996 instead of 1000 is adjustment, with 1000 it jumps ahead a minute every 3-4 hours

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);


WiFiClientSecure client;


void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  EEPROM.begin(400);
  //Display detection and config!
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  //delay(1000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 2);
  display.println("Hello,    EEE'16!");
  display.display();
  delay(500);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  //SD card testing
  if (!SD.begin(15)) {//cs to 15
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {

    Serial.println("No SD card attached");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(2, 10);
    display.println("No SD Card");
    display.display();
    delay(500);
    return;
  }
  //  //Create directory and CSV file for attendance
  //  appendFile(SD, "/EE1103/1.csv", "");//fs.open()
  //  appendFile(SD, "/Ph1103.csv", "");//fs.open()
  //  appendFile(SD, "/Math1103.csv", "");//fs.open()
  //  appendFile(SD, "/Ch1103.csv", "");//fs.open()
  //  appendFile(SD, "/Hum1103.csv", "");//fs.open()

  //String EE1103_csv=readFile(SD, "/EE1103.csv");

  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Fingerprint Sensor OK");
    display.display();
    delay(500);
    display.clearDisplay();
  }
  else {
    Serial.println("Did not find fingerprint sensor :(");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Fingerprint Sensor  NOT OK");
    display.display();
    delay(500);
    display.clearDisplay();
    //    while (1) {
    //      delay(1);
    //    }
  }

  //finger.getTemplateCount();
  //Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  //Serial.println("Waiting for valid finger...");
  pinMode(CLK, INPUT); pinMode(DT, INPUT); pinMode(SW, INPUT);
  previousCLK = digitalRead(CLK);
}

void loop() {
  Serial.print("For Enroll Press:: 1  \n");
  display.clearDisplay();
  counter = 1;
  int d;
  while (1) {
    get_rotation();
    if (counter >= 5) {
      counter = 1;
    }
    if (counter <= 0) {
      counter = 4;
    }

    if (counter == 2) {
      display.clearDisplay();
      display.setCursor(5, 10);
      display.setTextSize(2);
      display.println("New Enroll");
      display.display();
    }

    if (counter == 1) {
      display.clearDisplay();
      display.setCursor(5, 10);
      display.setTextSize(2);
      display.println("Attendence");
      display.display();
    }
    if (counter == 3) {
      display.clearDisplay();
      display.setCursor(40, 10);
      display.setTextSize(2);
      display.println("DHCP");
      display.display();
    }
    if (counter == 4) {
      display.clearDisplay();
      display.setCursor(30, 10);
      display.setTextSize(2);
      display.println("Update");
      display.setCursor(40, 30);
      display.println("Data");
      display.display();
    }

    if (digitalRead(SW) == LOW) {
      //    reset_OLED();
      d = counter;
      display.clearDisplay();
      delay(500);
      break;
    }

  }


  if (d == 1) {
    choosesub();
    Serial.println("Please, Place your Finger:");
    display.clearDisplay();
    display.setCursor(20, 10);
    display.setTextSize(2);
    display.println("Start");
    display.setCursor(2, 30);
    display.println("Attendance");
    display.display();
    while (1) {
      GetFingerprintID();
    }

  }

  if (d == 2) {
    Serial.println("Enter Password to New Fingerprint Enroll::");
    //if (pass == readnumber()) {
    while (1) {
      enroll();
    }
  }

  if (d == 3) {
    Serial.println("Ap mode activated");
    display.clearDisplay();

    display.setCursor(20, 10);
    display.setTextSize(1);
    display.println("DHCP Starting");

    display.setCursor(20, 30);
    display.setTextSize(1);
    display.println("SSID: ESP32WiFi");

    display.setCursor(20, 50);
    display.setTextSize(1);
    display.println("IP:192.168.4.1");
    display.display();
    while (loadWIFICredsForm());
    Serial.println(" ");
    Serial.println("Server Mode activated:");
    String ssid_read = EEPROM.readString(100);
    String pass_read = EEPROM.readString(200);
    ssid = ssid_read.c_str();
    pass = pass_read.c_str();
    Serial.print("Connecting to: ");
    Serial.print(ssid);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5, 10);
    display.println("Connecting To WiFi" + ssid_read);
    display.display();
    WiFi.begin(ssid, pass);
    int timeout = 2 * 10; // 10 seconds
    while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
      delay(500);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(5, 10);
      display.println("Connecting To WiFi" + ssid_read);
      display.display();
      Serial.print(".");
    }
  }

  if (d == 4) {
    //checking WiFi connections and also SD card data and update them
    connectingWifi();
    if (WiFi.status() == WL_CONNECTED) {
      chooseSubtoUpdate();
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(10, 10);
      display.println("Updating");
      display.setCursor(25, 30);
      display.println("Done");
      display.display();
      delay(1500);
    }
    else {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(15, 10);
      display.println("WiFi Not");
      display.setCursor(10, 30);
      display.println("Connected");
      display.display();
      delay(1500);
    }

  }

}

//ReadNumber


//GetFingerprint Enroll
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}



//getfingerprint ID
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}


void sendData(String params) {
  HTTPClient http;
  //GOOGLE_SCRIPT_ID = "AKfycbwC_6k8T-cjK1ociuH7rF67f8xEbvRULjxGIGh-Piz7wgyJjFPKadTzLBbu2dJPDauL";
  String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + params;
  Serial.print(url);
  Serial.println("Making a request");
  http.begin(url); //
  int httpCode = http.GET();
  http.end();
  Serial.println(": done " + httpCode);
}

void connectingWifi() {
  Serial.print("Connecting to: ");

  display.clearDisplay();
  if (!CheckWIFICreds()) {
    Serial.println("No WIFI credentials stored in memory. Loading form...");
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
    WiFi.mode(WIFI_AP);
    while (loadWIFICredsForm());
  }

  String ssid_read = EEPROM.readString(100);
  String pass_read = EEPROM.readString(200);
  ssid = ssid_read.c_str();
  pass = pass_read.c_str();
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 10);
  display.println("Connecting To WiFi");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 30);
  display.println(ssid_read);
  display.display();
  delay(500);
  WiFi.begin(ssid, pass);
  int timeout = 2 * 10; // 10 seconds
  while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect, going back to sleep");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5, 10);
    display.println("WiFi NOT Connected");
    display.display();
    delay(500);
    display.clearDisplay();
    return;
  }
  else {
    Serial.print("WiFi connected in: ");
    Serial.print(millis());
    Serial.print(", IP address: ");
    Serial.println(WiFi.localIP());
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5, 10);
    display.println("WiFi Connected");
    display.display();
    delay(500);
    display.clearDisplay();
  }
}

void chooseSubtoUpdate() {
  while (1) {
    display.clearDisplay();
    display.setCursor(25, 0);
    display.setTextSize(2);
    display.println("Please");
    display.println("Choose Sub");
    display.display();
    counter = 1;
    int pre_count = 0;
    int s;
    while (1) {
      get_rotation();
      if (counter != pre_count) {
        pre_count = counter;

        if (counter >= 6) {
          counter = 1;
        }
        if (counter <= 0) {
          counter = 5;
        }
        if (counter == 1) {
          display.clearDisplay();
          display.setCursor(25, 30);
          display.setTextSize(2);
          display.println("EE 1103 ");
          display.display();
        }
        if (counter == 2) {
          display.clearDisplay();
          display.setCursor(10, 30);
          display.setTextSize(2);
          display.println("Math 1103 ");
          display.display();
        }
        if (counter == 3) {
          display.clearDisplay();
          display.setCursor(25, 30);
          display.setTextSize(2);
          display.println("Ph 1103 ");
          display.display();
        }
        if (counter == 4) {
          display.clearDisplay();
          display.setCursor(25, 30);
          display.setTextSize(2);
          display.println("Ch 1103 ");
          display.display();
        }
        if (counter == 5) {
          display.clearDisplay();
          display.setCursor(25, 30);
          display.setTextSize(2);
          display.println("Hum 1103 ");
          display.display();
        }
        if (digitalRead(SW) == LOW) {
          //    reset_OLED();
          s = counter;
          Serial.print("S:");
          Serial.println(s);
          delay(500);
          display.clearDisplay();
          break;
        }
      }
      if (digitalRead(SW) == LOW) {
        //    reset_OLED();
        s = counter;
        Serial.print("S:");
        Serial.println(s);
        delay(500);
        display.clearDisplay();
        break;
      }
    }
    if (s == 1) {
      GOOGLE_SCRIPT_ID = GS_ID_EE1103;
      //SD_cardpath==/EE1103
      //if new class add a new csv file to the directory EE1103
      //if old class then append on the last class
      listDir(SD, "/EE1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (last_class_no == 0) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(15, 10);
        display.println("No Data");
        display.setCursor(10, 30);
        display.println("To Update");
        display.display();
      }
      else {
        GOOGLE_SCRIPT_ID = GS_ID_EE1103;
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(10, 10);
        display.println("Updating..");
        display.display();
        for (int i = 1; i <= last_class_no; i++) {
          String SD_cardpath = "/EE1103/" + String(i) + ".csv";
          const char *SD_cardpathcc = SD_cardpath.c_str();
          readFile(SD, SD_cardpathcc, i);
        }
      }
      break;
    }
    else if (s == 2) {
      GOOGLE_SCRIPT_ID = GS_ID_Math1103;
      listDir(SD, "/Math1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (last_class_no == 0) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(15, 10);
        display.println("No Data");
        display.setCursor(10, 30);
        display.println("To Update");
        display.display();
      }
      else {
        GOOGLE_SCRIPT_ID = GS_ID_Math1103;
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(10, 10);
        display.println("Updating..");
        display.display();
        for (int i = 1; i <= last_class_no; i++) {
          String SD_cardpath = "/Math1103/" + String(i) + ".csv";
          const char *SD_cardpathcc = SD_cardpath.c_str();
          readFile(SD, SD_cardpathcc, i);
        }
      }
      break;
    }

    else if (s == 4) {
      GOOGLE_SCRIPT_ID = GS_ID_Ch1103;
      listDir(SD, "/Ch1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (last_class_no == 0) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(15, 10);
        display.println("No Data");
        display.setCursor(10, 30);
        display.println("To Update");
        display.display();
      }
      else {
        GOOGLE_SCRIPT_ID = GS_ID_Ch1103;
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(10, 10);
        display.println("Updating..");
        display.display();
        for (int i = 1; i <= last_class_no; i++) {
          String SD_cardpath = "/EE1103/" + String(i) + ".csv";
          const char *SD_cardpathcc = SD_cardpath.c_str();
          readFile(SD, SD_cardpathcc, i);
        }
      }
      break;
    }
    else if (s == 3) {
      GOOGLE_SCRIPT_ID = GS_ID_Ph1103;
      listDir(SD, "/Ph1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (last_class_no == 0) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(15, 10);
        display.println("No Data");
        display.setCursor(10, 30);
        display.println("To Update");
        display.display();
      }
      else {
        GOOGLE_SCRIPT_ID = GS_ID_Ph1103;
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(10, 10);
        display.println("Updating..");
        display.display();
        for (int i = 1; i <= last_class_no; i++) {
          String SD_cardpath = "/Ph1103/" + String(i) + ".csv";
          const char *SD_cardpathcc = SD_cardpath.c_str();
          readFile(SD, SD_cardpathcc, i);
        }
      }
      break;
    }
    else if (s == 5) {
      GOOGLE_SCRIPT_ID = GS_ID_Hum1103;
      listDir(SD, "/Hum1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (last_class_no == 0) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(15, 10);
        display.println("No Data");
        display.setCursor(10, 30);
        display.println("To Update");
        display.display();
      }
      else {
        GOOGLE_SCRIPT_ID = GS_ID_Hum1103;
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(10, 10);
        display.println("Updating..");
        display.display();
        for (int i = 1; i <= last_class_no; i++) {
          String SD_cardpath = "/Hum1103/" + String(i) + ".csv";
          const char *SD_cardpathcc = SD_cardpath.c_str();
          readFile(SD, SD_cardpathcc, i);
        }
      }
      break;
    }
    else {
      Serial.println("Invalid Option:::");
      Serial.println("Please Select from 1 to 5:::");
      display.clearDisplay();
      display.setCursor(10, 10);
      display.setTextSize(2);
      display.println("Invalid");
      display.display();
      delay(1000);
      choosesub();
    }
  }
}
