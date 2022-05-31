void showLogo() { // show logo with (bad) fade-in and -out
	setBrightness(0);
	drawTexture(0, 0, logo, 128,64);
	display.display();
	//will delay for about 500ms
	for(uint8_t i=1;i!=0;++i){
		setBrightness(i);
		_delay_ms(2);
	}
	display.clearDisplay();
	display.display();
	_delay_ms(100);
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
 * @param	color1	first color
 * @param	color2	second color
 */
void drawTextureWithPalette(const uint8_t _x,const uint8_t _y,const uint8_t texture[],const uint8_t width,const uint8_t height,const uint16_t color1,const uint16_t color2){
	/* _y is the first y-coordinate
	 * y_ is the last y-coordinate
	 * y is the current y-coordinate
	 * same for x
	 */
	const uint8_t y_=_y+height;
	const uint8_t x_=_x+width;
	uint16_t i=0;//index in current array of uint8_t
	uint8_t j=0b10000000;//bitmask for current uint8_t
	uint8_t b=pgm_read_byte_near(texture+i);//current uint8_t
	uint8_t color;
	display.startWrite();
	for(uint8_t y=_y;y<y_;++y){
		for(uint8_t x=_x;x<x_;++x){
			//getting current color
			color = (j & b) ? color1 : color2;
			//not drawing transparent pixels
			if(color!=TRANSPARENT)
				display.drawPixel(x,y,color);
			//incrementing counters
			if(j==0b00000001){
				j=0b10000000;
				++i;
				b=pgm_read_byte_near(texture+i);
			}else{
				j >>= 1;
			}
		}
	}
	display.endWrite();
}

/*!
 * Draws a Pixel with scaling
 * @param x the x-coordinate - gets handled as the top left corner of the zoomed pixel
 * @param y same as x
 * @param zoom the scaling factor - scales like 1x1 2x2 4x4 8x8 etc. - zoom of 0 is UB
 */
void drawZoomedPixel(const uint8_t x,const uint8_t y,uint8_t zoom){
	zoom=1<<(zoom-1);//converting zoom to pixels drawn
	for(uint8_t i=0;i<zoom;++i)
		for(uint8_t j=0;j<zoom;++j)
			display.drawPixel(i+x*zoom,j+y*zoom,SSD1306_WHITE);
}

//TODO: add printFromFlash (see in menu.cpp)

/**
 * Draws the inner outline of a rectangle
 * @param x the x-coordinate of the top left corner of the rectangle
 * @param y the y-coordinate of the top left corner of the rectangle
 * @param width the width of the rectangle
 * @param height the height of the rectangle
 * @param color the color of the outline
 */
inline void drawRect(const uint8_t x,const uint8_t y,const uint8_t width,const uint8_t height,const uint8_t color){
	drawHLine(x,y,width,color);//top
	drawVLine(x,y+1,height-1,color);//left
	drawHLine(x+1,y+height-1,width-1,color);//bottom
	drawVLine(x+width-1,y+1,height-2,color);//right
}

/**
 * Fills a rectangle
 * @param x the x-coordinate of the top left corner of the rectangle
 * @param y the y-coordinate of the top left corner of the rectangle
 * @param width the width of the rectangle
 * @param height the height of the rectangle
 * @param color the color of the rectangle
 */
inline void fillRect(const uint8_t _x,const uint8_t _y,const uint8_t width,const uint8_t height,const uint8_t color){
	const uint8_t y_=_y+height;
	const uint8_t x_=_x+width;
	for(uint8_t x=_x;x<x_;++x)
		for(uint8_t y=_y;y<y_;++y)
			display.drawPixel(x,y,color);
}

//to reset everything to how games usually want it
void beginGame(){
	display.clearDisplay();
	display.setCursor(0, 0);
}

//to set everything to how the menu wants it
void endGame(){
	display.clearDisplay();
	display.setTextColor(SSD1306_WHITE);
}

uint32_t _smartSleepCheckPoint = 0;
/**
 * Sleeps until the given time has passed since the last call
 * since the first call doesn't wait, this should be added as the first call in the loop-to-limit
 * @param want_dt the given time in milliseconds
 */
void smartSleep(const uint32_t want_dt){
	uint32_t is_dt = millis()-_smartSleepCheckPoint;
	if(is_dt < want_dt){
		//sleeping off the leftover time while updating the buttons
		for(uint16_t x=(want_dt - is_dt)/10;x>0;x--){
			delay(10);
			up.tick();
			down.tick();
			left.tick();
			right.tick();
			sh_r.tick();
			sh_l.tick();
		}
	}
	_smartSleepCheckPoint += is_dt;//no new millis() for precision - and also speed
}

//0 is completely dark and 255 is completely bright
void setBrightness(uint8_t amount){
	SPI.beginTransaction(SPISettings(8000000UL,MSBFIRST,SPI_MODE0));
	digitalWrite(OLED_CS, LOW);
	digitalWrite(OLED_DC, LOW);
	SPI.transfer(SSD1306_SETCONTRAST);
	SPI.transfer(amount);
	digitalWrite(OLED_CS, HIGH);
	SPI.endTransaction();
}
