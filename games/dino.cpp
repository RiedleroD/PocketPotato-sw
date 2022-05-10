namespace dino {
	void game() {
		const uint8_t FLOOR_Y = 42;
		float y = FLOOR_Y;
		float vel = 0;
		unsigned long nextMove = 0;
		unsigned long lastMove = 0;
		unsigned long nextStep = 0;
		unsigned long startTime = millis();
		uint8_t animStep = 0;
		uint8_t* animFrames[] = {keksiLegs1,keksiLegs2,keksiLegs3,keksiLegs4};

		uint8_t cactx = 128;

		while (true) {
			//jump & ground check if down is not held
			if(up.state() && y == FLOOR_Y && !down.state())
				vel = 300;
			//faster fall if down is held
			if(down.state() && y < FLOOR_Y)
				vel -= 50;

			if(millis() > nextMove) {
				//moves the player
				if(vel!= 0 || y < FLOOR_Y) {
					y -= vel * ((millis() - lastMove)/1000.0);
					vel -= 1200 * ((millis() - lastMove)/1000.0);
				}
				lastMove = millis();

				//moves the cactus;
				cactx -= 5;
				if(cactx < 0)
					cactx = 128;
				nextMove = millis() + 20;
			}

			//resets Momentum on Ground
			if(y >= FLOOR_Y) {
				y = FLOOR_Y;
				vel = 0;
			}

			//collision detection
			if(cactx >= 20 && cactx <= 35 && y >= 22) {
				break;
			}

			//draw graphics
			display.clearDisplay();
			//Ground
			for(int i = 62; i < 64; i++) {
				for(int j = 0; j < 124; j++) {
					display.drawPixel(j, i, SSD1306_WHITE);
				}
			}

			drawTexture(cactx, FLOOR_Y, cactus, 15, 20);

			drawTexture(20, y, keksiHead, 15, 20);
			if(millis() > nextStep) {
				nextStep = millis() + 100;
				if(++animStep >= 4)
					animStep = 0;
			}
			drawTexture(20, y+15, animFrames[animStep], 15, 5);

			//diplays score
			display.setCursor(2,2);
			display.print(F("Score: "));
			display.print((millis()-startTime)/100);

			display.display();

			//return to menu
			if(left.isHold())
				return;
		}
		uint16_t score = (millis()-startTime)/100;

		//draws GameOver screen
		drawRect(32,16,64,32,WHITE);
		fillRect(33,17,62,30,BLACK);

		display.setCursor(37, 18);
		display.print(F("Game Over"));

		uint16_t highscore;
		EEPROM.get(ADR_DINOSCORE, highscore);
		if(score > highscore || highscore == 65535) {
			display.setCursor(37, 28);
			display.print(F("New Highscore"));
			EEPROM.put(ADR_DINOSCORE, score);
		}

		display.display();

		while(!left.state()) {}
	}
}
