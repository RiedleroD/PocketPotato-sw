void gameTest() {
	uint8_t x = 0;
	uint8_t y = 0;
	unsigned long nextPrint = 0;
	unsigned long nextMove = 0;
	
	for(x=1;x<128;++x){
		display.clearDisplay();
		display.drawPixel(x-1,0,WHITE);
		drawHLine(0,3,x,WHITE);
		drawRect(0,5,x,10,WHITE);
		fillRect(0,16,x,10,WHITE);
		display.display();
		delay(10);
	}
	for(y=1;y<64;++y){
		display.clearDisplay();
		display.drawPixel(0,y-1,WHITE);
		drawVLine(3,0,y,WHITE);
		drawRect(5,0,10,y,WHITE);
		fillRect(16,0,10,y,WHITE);
		display.display();
		delay(20);
	}
	for(x=0;x<64;++x){
		display.clearDisplay();
		display.fillRect(0,0,x+1,x+1,WHITE);
		display.drawRect(64,0,x+1,x+1,WHITE);
		display.drawPixel(0,0,INVERT);
		display.drawPixel(x,0,INVERT);
		display.drawPixel(0,x,INVERT);
		display.drawPixel(x,x,INVERT);
		display.display();
		delay(20);
	}
	
	x=0;y=0;
	
	while (true) {
		if(millis() > nextMove) {
			if(left.state())
				x = constrain(x - 1, 0, 127);
			if(right.state())
				x = constrain(x + 1, 0, 127);
			if(up.state())
				y = constrain(y - 1, 0, 63);
			if(down.state())
				y = constrain(y + 1, 0, 63);
			nextMove = millis() + 10;
		}


		if(millis() > nextPrint) {
			Serial.print(x);
			Serial.print("/");
			Serial.println(y);
			nextPrint = millis() + 1000;
		}

		display.clearDisplay();
		drawTexture(x, y, smile, 6, 4);

		display.display();
		if(up.isHold() && down.isHold())
			return;
	}
}
