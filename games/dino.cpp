namespace dino {
	void game() {
		float y = 42;
		float vel = 0;
		unsigned long nextPrint = 0;
		unsigned long nextMove = 0;
		unsigned long lastMove = 0;
		unsigned long nextStep = 0;
		unsigned long startTime = millis();
		uint8_t step = 0;

		uint8_t cactx = 128;

		while (true) {
			//input detection
			if(up.state() && y == 42 && !down.state())
				vel = 300;

			if(down.state() && y < 42)
				vel = -8;

			if(millis() > nextMove) {
				//moves the player
				if(vel!= 0 || y < 42) {
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
			if(y >= 42) {
				y = 42;
				vel = 0;
			}

			//collision detection
			if(cactx >= 20 && cactx <= 35 && y >= 22) {
				Serial.println("Fail!");
			}

			//draw graphics
			display.clearDisplay();
			//Ground
			for(int i = 62; i < 64; i++) {
				for(int j = 0; j < 124; j++) {
					display.drawPixel(j, i, SSD1306_WHITE);
				}
			}

			drawTexture(cactx, 42, cactus, 15, 20);

			drawTexture(20, y, keksiHead, 15, 20);
			if(millis() > nextStep) {
				nextStep = millis() + 100;
				if(++step >= 4)
					step = 0;
			}
			switch (step) {
				case 0:
					drawTexture(20, y+15, keksiLegs1, 15, 5);
					break;
				case 1:
					drawTexture(20, y+15, keksiLegs2, 15, 5);
					break;
				case 2:
					drawTexture(20, y+15, keksiLegs3, 15, 5);
					break;
				case 3:
					drawTexture(20, y+15, keksiLegs4, 15, 5);
					break;
			}

			//diplays score
			display.setCursor(2,2);
			display.print(F("Score: "));
			display.print((millis()-startTime)/10);

			display.display();

			//return to menu
			if(left.isHold())
				return;
		}
	}
}
