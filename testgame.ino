void gameTest() {
  display.clearDisplay();
  uint8_t x = 0;
  uint8_t y = 0;
  unsigned long nextTime = 0;

  while (true) {
    if(left.isClick() || left.isStep())
      x = constrain(x - 1, 0, 127);
    if(right.isClick() || right.isStep())
      x = constrain(x + 1, 0, 127);
    if(up.isClick() || up.isStep())
      y = constrain(y - 1, 0, 63);
    if(down.isClick() || down.isStep())
      y = constrain(y + 1, 0, 63);


    if(millis() > nextTime) {
      Serial.print(x);
      Serial.print("/");
      Serial.println(y);
      nextTime = millis() + 1000;
    }
    
    display.clearDisplay();
    drawTexture(x, y, smile, 6, 4);

    display.display();
    if(up.isHold() && down.isHold())
      return;
  }
}
