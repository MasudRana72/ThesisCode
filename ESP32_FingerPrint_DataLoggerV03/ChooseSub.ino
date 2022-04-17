void choosesub() {
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
    // Choose new or old class
    counter = 1;
    pre_count = 0;
    int class_flag = 0;
    int p;
    while (1) {
      get_rotation();
      if (counter != pre_count) {
        pre_count = counter;

        if (counter >= 3) {
          counter = 1;
        }
        if (counter <= 0) {
          counter = 2;
        }
        if (counter == 1) {
          display.clearDisplay();
          display.setCursor(5, 30);
          display.setTextSize(2);
          display.println("New Class?");
          display.display();
        }
        if (counter == 2) {
          display.clearDisplay();
          display.setCursor(5, 30);
          display.setTextSize(2);
          display.println("Old Class?");
          display.display();
        }
        if (digitalRead(SW) == LOW) {
          //    reset_OLED();
          p = counter;
          Serial.print("p:");
          Serial.println(p);
          delay(500);
          display.clearDisplay();
          break;
        }
      }
      if (digitalRead(SW) == LOW) {
        //    reset_OLED();
        p = counter;
        Serial.print("p:");
        Serial.println(p);
        delay(500);
        display.clearDisplay();
        break;
      }
    }

    if (p == 1) {
      class_flag = 1;
    }
    else {
      class_flag = 0;
    }



    Serial.println("Place Your Finger");

    if (s == 1) {
      GOOGLE_SCRIPT_ID = GS_ID_EE1103;
      //SD_cardpath==/EE1103
      //if new class add a new csv file to the directory EE1103
      //if old class then append on the last class
      listDir(SD, "/EE1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (class_flag) {
        String SD_cardpath = "/EE1103/" + String(last_class_no + 1) + ".csv";
        SD_cardpath_C = SD_cardpath;
        const char *SD_cardpathcc = SD_cardpath.c_str();
        String Messagee = "ID,FLAG\n";
        const char* cc_messagee = Messagee.c_str();
        appendFile(SD, SD_cardpathcc, cc_messagee);
      }
      else {
        String SD_cardpath = "/EE1103/" + String(last_class_no) + ".csv";
        SD_cardpath_C = SD_cardpath;
      }
      display.clearDisplay();
      display.setCursor(10, 10);
      display.setTextSize(2);
      display.println("EE 1103");
      display.display();
      delay(1000);
      display.clearDisplay();
      display.setCursor(20, 10);
      display.setTextSize(2);
      display.println("Please");
      display.setCursor(15, 30);
      display.setTextSize(2);
      display.println("Initiate");
      display.display();
      delay(1000);
      while (1) {
        getFingerprintIDez();
        if (finger.fingerID == 150) {
          display.clearDisplay();
          display.setCursor(2, 10);
          display.setTextSize(2);
          display.println("Authicated");
          display.setCursor(50, 30);
          display.setTextSize(2);
          display.println("<|>");
          display.display();
          delay(1000);
          break;
        }
      }
      break;
    }
    else if (s == 2) {
      GOOGLE_SCRIPT_ID = GS_ID_Math1103;
      listDir(SD, "/Math1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (class_flag) {
        String SD_cardpath = "/Math1103/" + String(last_class_no + 1) + ".csv";
        SD_cardpath_C = SD_cardpath;
        const char *SD_cardpathcc = SD_cardpath.c_str();
        String Messagee = "ID,FLAG\n";
        const char* cc_messagee = Messagee.c_str();
        appendFile(SD, SD_cardpathcc, cc_messagee);
      }
      else {
        String SD_cardpath = "/Math1103/" + String(last_class_no) + ".csv";
        SD_cardpath_C = SD_cardpath;
      }
      display.clearDisplay();
      display.setCursor(10, 10);
      display.setTextSize(2);
      display.println("Math 1103");
      display.display();
      delay(1000);
      display.clearDisplay();
      display.setCursor(20, 10);
      display.setTextSize(2);
      display.println("Please");
      display.setCursor(15, 30);
      display.setTextSize(2);
      display.println("Initiate");
      display.display();
      delay(1000);
      while (1) {
        getFingerprintIDez();
        if (finger.fingerID == 151) {
          display.clearDisplay();
          display.setCursor(2, 10);
          display.setTextSize(2);
          display.println("Authicated");
          display.setCursor(50, 30);
          display.setTextSize(2);
          display.println("<|>");
          display.display();
          delay(1000);
          break;
        }
      }
      break;
    }

    else if (s == 4) {
      GOOGLE_SCRIPT_ID = GS_ID_Ch1103;
      listDir(SD, "/Math1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (class_flag) {
        String SD_cardpath = "/Ch1103/" + String(last_class_no + 1) + ".csv";
        SD_cardpath_C = SD_cardpath;
        const char *SD_cardpathcc = SD_cardpath.c_str();
        String Messagee = "ID,FLAG\n";
        const char* cc_messagee = Messagee.c_str();
        appendFile(SD, SD_cardpathcc, cc_messagee);
      }
      else {
        String SD_cardpath = "/Ch1103/" + String(last_class_no) + ".csv";
        SD_cardpath_C = SD_cardpath;
      }
      display.clearDisplay();
      display.setCursor(10, 10);
      display.setTextSize(2);
      display.println("Ch 1103");
      display.display();
      delay(1000);
      display.clearDisplay();
      display.setCursor(20, 10);
      display.setTextSize(2);
      display.println("Please");
      display.setCursor(15, 30);
      display.setTextSize(2);
      display.println("Initiate");
      display.display();
      delay(1000);
      while (1) {
        getFingerprintIDez();
        if (finger.fingerID == 152) {
          display.clearDisplay();
          display.setCursor(2, 10);
          display.setTextSize(2);
          display.println("Authicated");
          display.setCursor(50, 30);
          display.setTextSize(2);
          display.println("<|>");
          display.display();
          delay(1000);
          break;
        }
      }
      break;
    }
    else if (s == 3) {
      GOOGLE_SCRIPT_ID = GS_ID_Ph1103;
      listDir(SD, "/Ph1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (class_flag) {
        String SD_cardpath = "/Math1103/" + String(last_class_no + 1) + ".csv";
        SD_cardpath_C = SD_cardpath;
        const char *SD_cardpathcc = SD_cardpath.c_str();
        String Messagee = "ID,FLAG\n";
        const char* cc_messagee = Messagee.c_str();
        appendFile(SD, SD_cardpathcc, cc_messagee);
      }
      else {
        String SD_cardpath = "/Ph1103/" + String(last_class_no) + ".csv";
        SD_cardpath_C = SD_cardpath;
      }
      display.clearDisplay();
      display.setCursor(10, 10);
      display.setTextSize(2);
      display.println("Ph 1103");
      display.display();
      delay(1000);
      display.clearDisplay();
      display.setCursor(20, 10);
      display.setTextSize(2);
      display.println("Please");
      display.setCursor(15, 30);
      display.setTextSize(2);
      display.println("Initiate");
      display.display();
      delay(1000);
      while (1) {
        getFingerprintIDez();
        if (finger.fingerID == 153) {
          display.clearDisplay();
          display.setCursor(2, 10);
          display.setTextSize(2);
          display.println("Authicated");
          display.setCursor(50, 30);
          display.setTextSize(2);
          display.println("<|>");
          display.display();
          delay(1000);
          break;
        }
      }
      break;
    }
    else if (s == 5) {
      GOOGLE_SCRIPT_ID = GS_ID_Hum1103;
      listDir(SD, "/Hum1103", 0);
      Serial.print("Toatal class:: ");
      Serial.println(last_class_no);
      if (class_flag) {
        String SD_cardpath = "/Math1103/" + String(last_class_no + 1) + ".csv";
        SD_cardpath_C = SD_cardpath;
        const char *SD_cardpathcc = SD_cardpath.c_str();
        String Messagee = "ID,FLAG\n";
        const char* cc_messagee = Messagee.c_str();
        appendFile(SD, SD_cardpathcc, cc_messagee);
      }
      else {
        String SD_cardpath = "/Hum1103/" + String(last_class_no) + ".csv";
        SD_cardpath_C = SD_cardpath;
      }
      display.clearDisplay();
      display.setCursor(10, 10);
      display.setTextSize(2);
      display.println("Hum 1103");
      display.display();
      delay(1000);
      display.clearDisplay();
      display.setCursor(20, 10);
      display.setTextSize(2);
      display.println("Please");
      display.setCursor(15, 30);
      display.setTextSize(2);
      display.println("Initiate");
      display.display();
      delay(1000);
      while (1) {
        getFingerprintIDez();
        if (finger.fingerID == 154) {
          display.clearDisplay();
          display.setCursor(2, 10);
          display.setTextSize(2);
          display.println("Authicated");
          display.setCursor(50, 30);
          display.setTextSize(2);
          display.println("<|>");
          display.display();
          delay(1000);
          break;
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

void get_rotation() {
  currentCLK = digitalRead(CLK);
  if (currentCLK != previousCLK)
  {
    if (digitalRead(DT) != currentCLK)
    {
      counter++;
      Serial.print("clockwise:"); Serial.println(counter);
      encdir = "clockwise";
    }
    //    else
    //    {
    //      counter++;
    //      Serial.print("counterclk:"); Serial.println(counter);
    //      encdir = "counterclk";
    //    }
    previousCLK = currentCLK;
  }

}
