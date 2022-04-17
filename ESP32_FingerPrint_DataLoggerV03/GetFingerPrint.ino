void GetFingerprintID() {
  getFingerprintIDez();
  for (int i = 1; i <= 127; i++) {
    if (finger.fingerID == i) {
      Serial.print("!!--");
      Serial.println(finger.fingerID);
      int Roll;
      String flag;
      Roll = i + 1603000;
      NAME = String(Roll);
      ID = "i";
      if (finger.confidence >= 60) {
        display.clearDisplay();
        display.setCursor(0, 10);
        display.setTextSize(2);
        display.println("ID Found");
        display.display();
        Serial.print("Attendace Marked for "); Serial.println(NAME);
        flag = "0";
        //SD_cardpathcc = "/EE1103/3.csv";
        const char *SD_cardpathcc_C = SD_cardpath_C.c_str();
        String Message = NAME + "," + flag + "\n";
        const char* cc_message = Message.c_str();
        appendFile(SD, SD_cardpathcc_C, cc_message);
        //sendData("NAME=" + NAME);
        display.clearDisplay();
        display.setCursor(5, 10);
        display.setTextSize(2);
        display.println(NAME);
        display.println("Attendence  Taken");
        display.display();
        delay(2000);
        display.clearDisplay();
        display.setCursor(0, 10);
        display.setTextSize(2);
        display.println("Ready For The Next");
        display.display();

      }
    }
  }
  finger.fingerID = 0;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
