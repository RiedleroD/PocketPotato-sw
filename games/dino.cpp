namespace dino {
	void game() {
		uint8_t y = 42;
		int8_t vel = 0;
		unsigned long nextPrint = 0;
		unsigned long nextMove = 0;
		unsigned long nextStep = 0;
		uint8_t step = 0;

		uint8_t cactx = 128;

		while (true) {
			if(up.state() && y == 42 && !down.state())
				vel = 8;

			if(down.state() && y < 42)
				vel = -8;

			if(millis() > nextMove) {
				if(vel != 0 || y < 42) {
					y -= vel;
					vel--;
				}
				cactx--;
				if(cactx < 0)
					cactx;
				nextMove = millis() + 20;
			}

			if(y >= 42) {
				y = 42;
				vel = 0;
			}

			//DEBUG
			if(millis() > nextPrint) {
				Serial.print(y);
				Serial.print(", ");
				Serial.println(vel);
				nextPrint = millis() + 1000;
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

			display.display();

			//return to menu
			if(left.isHold())
				return;
		}
	}
}
