void checkEEPROM() {
  if (EEPROM[KEY_ADDR] != EEPROM_KEY) { // Check for EEPROM key. If it is not set, set some values to default
    EEPROM[KEY_ADDR] = EEPROM_KEY;
    EEPROM[EEPROM_CONTR] = 255; // Contrast
    EEPROM[EEPROM_POFF] = 1; // Power off interval (1 Min)
    for (uint8_t i = 0; i < APPS; i++) { // Fill game cells with 0s
      EEPROM[pgm_read_byte(&(app_eeprom_keys[i]))] = 0; // Set EEPROM cell with address <PROGMEM app cell> to 0
    }
  }
  else {  // Else leave cells unchanged and load contrast and sleep timeout
    display.dim(false); // Set OLED contrast to full
    // TODO: Sleep timeout
  }
}

void showLogo() { // show logo with (bad) fade-in and -out
	display.dim(true);
	drawTexture(0, 0, logo, 128, 64);
	display.display();
	_delay_ms(250);
	display.dim(false);
	_delay_ms(500);
	display.dim(true);
	_delay_ms(250);
	display.clearDisplay();
	display.display();
}

/*!
 * Draws a texture to screen, where one bit equals one pixel
 * no boundary checking is being done, keep this in mind!
 * the texture goes from left to right, top to bottom.
 * @param	_x		left boundary on the x-axis
 * @param	_y		top boundary on the y-axis
 * @param	texture	texture to be drawn
 * @param	width	width of the texture
 * @param	height	height of the texture
 * TODO: parameter to draw white as transparent
 */
void drawTexture(uint8_t _x,uint8_t _y,const uint8_t texture[],uint8_t width,uint8_t height){
	/* _y is the first y-coordinate
	 * y_ is the last y-coordinate
	 * y is the current y-coordinate
	 * same for x
	 */
	const uint8_t y_=_y+height;
	const uint8_t x_=_x+width;
	uint8_t i=0;//index of current uint8_t
	uint8_t j=0;//index in current uint8_t
	display.startWrite();
	for(uint8_t y=_y;y<y_;++y){
		for(uint8_t x=_x;x<x_;++x){
			//reading single bit from texture array;
			//pretty sure the lib knows that we're working with a single-bit
			//color & we don't need values larger than 1 to draw pixels
			display.writePixel(x,y,( 1 & (texture[i] >> j) ));
		}
		//incrementing counters
		if(j>=15){
			j=0;
			++i;
		}else{
			++j;
		}
	}	
	display.endWrite();
}

/*void drawMenu() {
  // If we have more than 7 apps, we want to scroll first (cursor on first line and items move)
  // and then stop the list and only move the cursor to the last element
  // If we have <= 7 elements, we can proceed with cursor only, without scrolling
  // E.g
  // APPS  menu  MODE
  // 3   0   cursor
  // 7   0   cursor
  // 9   0   scroll
  // 9   3   cursor
  static uint32_t menu_update = millis();

  if (millis() - menu_update >= (1000 / MENU_FPS)) {  // 5FPS update, more not needed
    menu_update = millis(); // Reset timer
    char arrayBuf[16];  // Buffer for menu item
    oled.clear();
    for (uint8_t i = ((APPS - menu > 6) ? menu : constrain(APPS - 7, 0, 7)), k = 1; i < APPS; i++, k++) {
      // If we have more than 7 elements, we print next app on first line. Else, we leave the list "as it is"
      // and only move the cursor
      strcpy_P(arrayBuf, pgm_read_word(&(apps[i])));  // Copy PROGMEM app into RAM (buffer)
      oled.setCursor(10, k); oled.print(arrayBuf); // Print
    }
    oled.setCursor(0, (APPS - menu > 6) ? 1 : ((menu > 7) ? menu - 1 : menu)); oled.print('>');
    // X - 0; Y - depends on current mode: scroll - stay on first row, cursor - move the cursor down
    //oled.drawBitmap(128 - 32, 64 - 16, console, 32, 16);
    oled.update();
  }
}*/
