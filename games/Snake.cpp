namespace snake{
	const char _menu0[] PROGMEM = "Start";
	const char _menu1[] PROGMEM = "Score";
	const char _menu2[] PROGMEM = "Guide";
	const char _menu3[] PROGMEM = "Close";
	
	const char* const menus[] PROGMEM = {_menu0,_menu1,_menu2,_menu3};
	const int menucount PROGMEM = sizeof(menus)/sizeof(char*);
	
	const char manual[] PROGMEM = "Collect the apples and get longer without colliding with yourself.\n\nUP+DOWN closes the game.";
	
	uint8_t getPartMaskOffset(uint8_t index){
		return 2*(index%4);
	}
	uint8_t getPartMask(uint8_t index){
		return 0b11000000 >> getPartMaskOffset(index);
	}
	#define getBlock(index) parts[(index/4) % partsLen]
	#define getPart(index) (getBlock(index) >> 6-getPartMaskOffset(index)) & 0b11
	#define setPart(index,value) getBlock(index) = \
									(getBlock(index) & (~getPartMask(index))) \
									| (value << \
										(6-getPartMaskOffset(index)) \
									  )
	inline uint16_t getScore(uint8_t partAmnt,uint8_t speed,uint8_t zoom){
		return (partAmnt-3)*speed/(1 << (zoom-1));
	}
	void genApples(uint8_t* arr,const uint8_t zoom){
		do{
			const uint8_t tmp = 0b11111111 >> zoom;
			arr[0]=1+random(tmp-1);
			arr[1]=1+random((tmp >> 1) -1);
		}while(arr[1]<<(zoom-1) <= 7 && arr[0]<<(zoom-1) <= 59);
	}
	void game(const uint8_t zoom,const uint8_t speed){
		//clamping coords to half of previous each zoom step
		const uint8_t coordMask = 0xFF >> zoom;
		const uint8_t coordMask1= coordMask >> 1;
		//the data stored for one part is just the direction the part is offset to, relative to the previous part.
		//00→left, 01→top, 10→right, 11→bottom
		//one part = 2 bits → 32 bytes = 512 parts
		//less parts are needed on greater zoom levels: 1→512, 2→256, 3→128
		//TODO: should expand to a greater size if needed
		const uint8_t partsLen=64/(1 << (zoom-1));
		uint8_t parts[partsLen] = {};
		//technically it's the amount of parts including the head part.
		uint8_t partAmnt = 3;
		//the offset wanders backwards in the array, reducing the amount of bits having to be written
		//when the snake moves
		uint16_t partOffset = partsLen*4 - 1;
		//first coord is the X coordinate (taking 7 bits, max value=127)
		//second coord is the Y coordinate (taking 6 bits, max value=63) and the direction (taking 2 bits)
		//I could possibly merge this into one uint16_t, but idk which variant is faster.
		uint8_t curCoords[2] = {(128 >> zoom)-1,(64 >> zoom)-1};
		//setting initial apple coords
		uint8_t appleCoords[2] = {0,0};
		genApples(appleCoords,zoom);
		uint32_t t1;
		//setting color to invert what's behind it
		display.setTextColor(SSD1306_INVERSE);
		//boolean to signal when you died
		bool dead = false;
		//I use blocks here to eliminate temporary variables when they're not needed anymore
		while(true){
			t1 = millis();
			//drawing snake + tail-related collision detection
			{
				display.clearDisplay();
				uint8_t curX=curCoords[0];
				uint8_t curY=curCoords[1] & coordMask1;
				for(uint8_t curI=partOffset;curI<(partOffset+partAmnt);++curI){
					//drawing the part
					drawZoomedPixel(curX,curY,zoom);
					//getting the next part
					switch(getPart(curI)){
						case 0b00://left
							--curX;
							curX &= coordMask;
							break;
						case 0b01://top
							--curY;
							curY &= coordMask1;
							break;
						case 0b10://right
							++curX;
							curX &= coordMask;
							break;
						case 0b11://bottom
							++curY;
							curY &= coordMask1;
							break;
					}
					//collision detection
					if((curX == curCoords[0]) && (curY == (curCoords[1]&coordMask1))){
						//dieing if colliding with head
						dead = true;
					}else if((curX == appleCoords[0]) && (curY == appleCoords[1])){
						//regenerating apple if colliding with it
						genApples(appleCoords,zoom);
					}
				}
			}
			//drawing apple
			drawZoomedPixel(appleCoords[0],appleCoords[1],zoom);
			//score/highscore stuff + dieing
			{
				//drawing score
				display.setCursor(0,0);
				display.print(F("Score: "));
				uint16_t score = getScore(partAmnt,speed,zoom);
				display.print(score);
				//ending game if appropriate
				if(dead){
					uint16_t hiscore;
					EEPROM.get(ADR_SNAKESCORE,hiscore);
					Serial.print(score);
					Serial.print(' ');
					Serial.println(hiscore);
					if(hiscore<score){//checking for highscore
						display.setCursor(34,32);//centered text
						display.setTextColor(SSD1306_WHITE,SSD1306_BLACK);//overwrites background
						display.print(F("HIGHSCORE!"));
						display.display();
						EEPROM.put(ADR_SNAKESCORE,score);
					}
					_delay_ms(500);
					return;//DIE
				}
			}
			//flipping display
			display.display();
			//movement
			{
				//input
				uint8_t direction = curCoords[1] >> 6;
				if(left.isClick() && direction!=0b00)
					direction=0b10;
				else if(up.isClick() && direction!=0b01)
					direction=0b11;
				else if(right.isClick() && direction!=0b10)
					direction=0b00;
				else if(down.isClick() && direction!=0b11)
					direction=0b01;
				else if(down.isHold() && up.isHold())
					return;
				//moving head
				switch(direction){
					case 0b00://right
						// (decreasing coordinate) % clamp coordinate
						curCoords[0] = (curCoords[0]+1) & coordMask;
						curCoords[1] &= coordMask1;
						break;
					case 0b01://bottom
						// (bitmasking direction) | (bitmasking and decreasing coordinate)
						curCoords[1] = (direction << 6) | ((curCoords[1]+1) & coordMask1);
						break;
					case 0b10://left
						curCoords[0] = (curCoords[0]-1) & coordMask;
						curCoords[1] = (curCoords[1] & coordMask1) | 0b10000000;
						break;
					case 0b11://top
						curCoords[1] = (direction << 6) | ((curCoords[1]-1) & coordMask1);
						break;
				}
				//moving parts
				if(partOffset==0)
					partOffset = 4*partsLen - 1;
				else
					--partOffset;
				//writing the current direction to the parts list.
				setPart(partOffset,direction);
			}
			//apple collision & regeneration
			if(((curCoords[0] == appleCoords[0]) && ((curCoords[1] & coordMask1) == appleCoords[1]))){
				genApples(appleCoords,zoom);
				//add one length
				++partAmnt;
			}
			//throttle frametime to set speed
			while((millis()-t1)<(60-5*speed)){
				up.tick();
				down.tick();
				left.tick();
				right.tick();
				sh_r.tick();
				sh_l.tick();
			}
		}
	}
	void showScore(){
		display.clearDisplay();
		display.setCursor(10,27);
		display.print(F("Highscore: "));
		{
			const uint16_t hiscore;
			EEPROM.get(ADR_SNAKESCORE,hiscore);
			display.print(hiscore);
		}
		display.display();
		while(true){
			if(left.isClick() || sh_l.isClick())
				break;
		}
	}
	void showManual(){
		display.clearDisplay();
		display.setCursor(0,0);
		for (uint16_t i=0;i < strlen_P(manual);++i){
			display.print((char)pgm_read_byte_near(manual + i));
		}
		display.display();
		while(true){
			if(left.isClick() || sh_l.isClick())
				break;
		}
	}
	void run(){
		while(true){
			switch(menu::draw(menus,menucount)){
				case 0:
					game(2,5);
					break;
				case 1:
					showScore();
					break;
				case 2:
					showManual();
					break;
				case 3:
					return;
			}
		}
	}
	#undef getBlock
	#undef getPart
	#undef setPart
}