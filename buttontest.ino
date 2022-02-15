/**
 * This programm is to test if the buttons work correctly.
 */

void testButtons() {
  display.clearDisplay();
  display.setCursor(0, 0);
  while (true) {
    if (down.isClick() || down.isStep())
      display.print("DOWN");
    else if (up.isClick() || up.isStep())
      display.print("UP");
    else if (left.isClick()) {
      display.print("LEFT");
    }
    else if (right.isClick() || sh_r.isClick()) {
      display.print("RIGHT");
    }
    display.display();

    if(up.isHold() && down.isHold()) {
      return;
    }
  }
}
