//extern void drawTexture(const uint8_t _x,const uint8_t _y,const uint8_t texture[],const uint8_t width,const uint8_t height);

namespace menu{
	void draw() {
		uint8_t select = 0;
		//const char texts[][20] = {"jahwe", "Cheesus", "jod", "readler", "sehrgey", "stainberger", "pushmän", "ceeplusplus", "käbab"};
		const uint8_t maxIndex = 8;

		while(true) {
			//moving of index
			if(up.isClick() || up.isStep())
				if(--select < 0)
					select = maxIndex;
			if(down.isClick() || down.isStep())
				if(++select > maxIndex)
					select = 0;

			//drawing of menu
			display.clearDisplay();
			for(uint8_t index = 0; index < 6; index++) {
				display.setCursor(10, 10 * index);
				display.print("texts[index]");
			}
			drawTexture(1, select * 10, arrow, 5, 9);
			display.display();
		}
	}

	/*void drawedMenu() {
		char arrayBuf[16];  // Buffer for menu item
		display.clearDisplay();
		for(uint8_t curApp=0;curApp<appcount;++curApp){
			//copy current app name into buffer
			strcpy_P(arrayBuf, pgm_read_word(&(apps[curApp])));
			//write current app name to screen
			display.setCursor(10,curApp*10);
			display.print(arrayBuf);
		}
		//write arrow
		drawTexture(1,menu*10,arrow,5,9);
		display.display();
	}*/
}
