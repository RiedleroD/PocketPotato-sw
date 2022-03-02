void gameTest() {
	uint8_t x = 0;
	uint8_t y = 0;
	unsigned long nextPrint = 0;
	unsigned long nextMove = 0;

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
