namespace snake{
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
	void game(){
		//the data stored for one part is just the direction the part is offset to, relative to the previous part.
		//00→left, 01→top, 10→right, 11→bottom
		//one part = 2 bits → 32 bytes = 512 parts
		//TODO: should expand to a greater size if needed
		const uint8_t partsLen=32;
		uint8_t parts[partsLen] = {};
		//technically it's the amount of parts including the head part.
		uint8_t partAmnt = 3;
		//the offset wanders backwards in the array, reducing the amount of bits having to be written
		//when the snake moves
		uint16_t partOffset = partsLen*4 - 1;
		//first coord is the X coordinate (taking 7 bits, max value=127)
		//second coord is the Y coordinate (taking 6 bits, max value=63) and the direction (taking 2 bits)
		//I could possibly merge this into one uint16_t, but idk which variant is faster.
		uint8_t curCoords[2] = {63,31};
		//setting initial apple coords so that they will be regenerated immediately
		uint8_t appleCoords[2] = {62,31};
		//speed will be set by a menu eventually - goes from 1 to 10
		uint8_t speed = 5;
		uint32_t t1;
		//I use blocks here to eliminate temporary variables when they're not needed anymore
		while(true){
			t1 = millis();
			//drawing snake + tail-related collision detection
			{
				display.clearDisplay();
				uint8_t curX=curCoords[0];
				uint8_t curY=curCoords[1] & 0b00111111;
				for(uint8_t curI=partOffset;curI<(partOffset+partAmnt);++curI){
					//drawing the part
					display.drawPixel(curX,curY,SSD1306_WHITE);
					//getting the next part
					switch(getPart(curI)){
						case 0b00://left
							--curX;
							curX &= 0b01111111;
							break;
						case 0b01://top
							--curY;
							curY &= 0b00111111;
							break;
						case 0b10://right
							++curX;
							curX &= 0b01111111;
							break;
						case 0b11://bottom
							++curY;
							curY &= 0b00111111;
							break;
					}
					//collision detection
					if((curX == curCoords[0]) && (curY == (curCoords[1]&0b00111111))){
						//dieing if colliding with head
						_delay_ms(500);
						return;//DIE
					}else if((curX == appleCoords[0]) && (curY == appleCoords[1])){
						//regenerating apple if colliding with it
						appleCoords[0]=1+random(126);
						appleCoords[1]=1+random(62);
					}
				}
			}
			//drawing apple
			display.drawPixel(appleCoords[0],appleCoords[1],SSD1306_WHITE);
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
						curCoords[0] = (curCoords[0]+1) & 0b01111111;
						curCoords[1] &= 0b00111111;
						break;
					case 0b01://bottom
						// (bitmasking direction) | (bitmasking and decreasing coordinate)
						curCoords[1] = (direction << 6) | ((curCoords[1]+1) & 0b00111111);
						break;
					case 0b10://left
						curCoords[0] = (curCoords[0]-1) & 0b01111111;
						curCoords[1] = (curCoords[1] & 0b00111111) | 0b10000000;
						break;
					case 0b11://top
						curCoords[1] = (direction << 6) | ((curCoords[1]-1) & 0b00111111);
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
			if(((curCoords[0] == appleCoords[0]) && ((curCoords[1] & 0b00111111) == appleCoords[1]))){
				//regenerate apple
				appleCoords[0]=1+random(126);
				appleCoords[1]=1+random(62);
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
}