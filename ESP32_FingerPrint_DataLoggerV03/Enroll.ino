void enroll() {
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 1000) you want to save this finger as...");
  //id = readnumber();
  //For Students
  //For teachers
  counter = 1;
  int d;
  while (1) {
    get_rotation();
    if (counter >= 3) {
      counter = 1;
    }
    if (counter <= 0) {
      counter = 2;
    }

    if (counter == 1) {
      display.clearDisplay();
      display.setCursor(0, 10);
      display.setTextSize(2);
      display.println("Students?");
      display.display();
    }

    if (counter == 2) {
      display.clearDisplay();
      display.setCursor(0, 10);
      display.setTextSize(2);
      display.println("Teachers?");
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
    int batch = 10;
    String batch_s = "2k" + String(batch);
    display.clearDisplay();
    display.setCursor(4, 10);
    display.setTextSize(2);
    display.println("Enter Batch");
    display.setCursor(35, 30);
    display.setTextSize(2);
    display.println(batch_s);
    display.display();
    counter = 1;
    int pre_count = 0;
    while (1) {
      get_rotation();
      if (counter > pre_count) {
        batch++;
        batch_s = "2k" + String(batch);
        pre_count = counter;
      }
//      else {
//        batch--;
//        batch_s = "2k" + String(batch);
//        pre_count = counter;
//      }
      display.clearDisplay();
      display.setCursor(0, 10);
      display.setTextSize(2);
      display.println("EnterBatch");
      display.setCursor(40, 30);
      display.setTextSize(2);
      display.println(batch_s);
      display.display();


      if (digitalRead(SW) == LOW) {
        Serial.print("batch:");
        Serial.println(batch);
        delay(500);
        display.clearDisplay();
        break;
      }

      if (digitalRead(SW) == LOW) {
        Serial.print("batch:");
        Serial.println(batch);
        delay(500);
        display.clearDisplay();
        break;
      }
    }

    //////////////////////Rolll

    int roll = 0;
    String roll_s = String(roll);
    display.clearDisplay();
    display.setCursor(4, 10);
    display.setTextSize(2);
    display.println(" Enter Roll");
    display.setCursor(45, 35);
    display.setTextSize(2);
    display.println(roll_s);
    display.display();
    counter = 2;
    pre_count = 0;
    while (1) {
      get_rotation();
      if (counter != pre_count) {
        roll++;
        if (roll >= 122) {
          roll = 1;
        }
        roll_s = String(roll);
        pre_count = counter;
      }
//      else {
//        roll--;
//        roll_s = String(roll);
//        pre_count = counter;
//      }
      display.clearDisplay();
      display.setCursor(0, 10);
      display.setTextSize(2);
      display.println("Enter Roll");
      display.setCursor(45, 35);
      display.setTextSize(2);
      display.println(roll_s);
      display.display();


      if (digitalRead(SW) == LOW) {
        Serial.print("roll:");
        Serial.println(roll);
        delay(500);
        display.clearDisplay();
        break;
      }
      if (digitalRead(SW) == LOW) {
        Serial.print("roll:");
        Serial.println(roll);
        delay(500);
        display.clearDisplay();
        break;
      }
    }
    id = roll;
    if (id == 0) {// ID #0 not allowed, try again!
      return;
    }
    Serial.print("Enrolling ID #");
    Serial.println(id);

    while (!  getFingerprintEnroll() );
  }
  if (d == 2) {
    display.clearDisplay();
    display.setCursor(25, 0);
    display.setTextSize(2);
    display.println("Please");
    display.println("Choose Teacher");
    display.display();

    counter = 1;
    int pre_count = 0;
    int s;
    int teacher_id;
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
        teacher_id = 149 + counter;
        Serial.print("S:");
        Serial.println(s);
        delay(500);
        display.clearDisplay();
        break;
      }
    }
    id = teacher_id;
    while (!  getFingerprintEnroll() );
  }
}



uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(2);
  display.println("Place Your");
  display.setCursor(20, 35);
  display.setTextSize(2);
  display.println("Finger");
  display.display();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        display.clearDisplay();
        display.setCursor(0, 10);
        display.setTextSize(2);
        display.println("Sensor Error");
        display.setCursor(0, 35);
        display.setTextSize(2);
        display.println("Reset Device");
        display.display();
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!



  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      display.clearDisplay();
      display.setCursor(0, 10);
      display.setTextSize(2);
      display.println("Sensor Error");
      display.setCursor(0, 35);
      display.setTextSize(2);
      display.println("Reset Device");
      display.display();
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

  Serial.println("Remove finger");
  display.clearDisplay();
  display.setCursor(20, 10);
  display.setTextSize(2);
  display.println("Remove");
  display.setCursor(20, 35);
  display.setTextSize(2);
  display.println("Finger");
  display.display();
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(2);
  display.println("Place Your");
  display.setCursor(0, 35);
  display.setTextSize(2);
  display.println("Finger Again");
  display.display();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        display.clearDisplay();
        display.setCursor(0, 10);
        display.setTextSize(2);
        display.println("Sensor Error");
        display.setCursor(0, 35);
        display.setTextSize(2);
        display.println("Reset Device");
        display.display();
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      display.clearDisplay();
      display.setCursor(0, 10);
      display.setTextSize(2);
      display.println("Sensor Error");
      display.setCursor(0, 35);
      display.setTextSize(2);
      display.println("Reset Device");
      display.display();
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
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(2);
    display.println("Sensor Error");
    display.setCursor(0, 35);
    display.setTextSize(2);
    display.println("Reset Device");
    display.display();
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println("Fingerprints did not match");
    display.display();
    delay(1000);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(2);
    display.println("Fingerprints Stored");
    display.display();
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(2);
    display.println("Sensor Error");
    display.setCursor(0, 35);
    display.setTextSize(2);
    display.println("Reset Device");
    display.display();
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}
