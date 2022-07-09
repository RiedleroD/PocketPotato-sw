#include "funcs.h"

void showLogo() { // show logo with (bad) fade-in and -out
	setBrightness(0);
	drawCompressedTexture(0,0,logo_compressed,128,64,WHITE,BLACK);
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
 * Draws a compressed texture to screen
 * the texture goes from left to right, top to bottom.
 * WARNING: barely any boundary checking
 * @param	_x		left boundary on the x-axis
 * @param	_y		top boundary on the y-axis
 * @param	texture	texture to be drawn
 * @param	width	width of the texture
 * @param	color1	primary color
 * @param	color2	secondary color
 */
void drawCompressedTexture(const uint8_t _x,const uint8_t _y,const uint8_t texture[],const uint8_t width,const uint8_t height,const uint16_t color1,const uint16_t color2){
	struct DeltaRegister{//lets me easily access them while also staying compact
		bool r1: 1,
		     r2: 1;
		//this is brilliant! Sometimes I really love C++.
	};

	enum State{PLAIN,RLE};//Plain and Run-length-encoding packets
	enum Mode{R,ER,DR,DDR};//RLE, Delta-Encode + RLE, Delta-Decode + RLE, Delta-Decode*2 + RLE
	
	//reading header
	const Mode mode = bitops::read_n(0,2,texture);
	const uint8_t block_size = bitops::read_n(2,3,texture);
	State state = bitops::read_single(5,texture);
	DeltaRegister delta_flags = {0,0};
	
	auto delta_context_helper= [&](bool bit){
		switch(mode){
			case Mode::ER:
				bool prevr1 = delta_flags.r1;
				delta_flags.r1 = bit;
				return (bool)(bit ^ prevr1);
			case Mode::R:
				return bit;
			case Mode::DDR:
				delta_flags.r2 ^= bit;
				bit = delta_flags.r2;
				//purposefully no break nor return;
			case Mode::DR:
				delta_flags.r1 ^= bit;
				return delta_flags.r1;
		}
	};
	//indices for within the compressed texture and the output
	uint16_t in_i = 6;//header is 6 bit long, so we start at index 6
	uint16_t out_i = 0;
	
	display.startWrite();
	while(out_i < width*height){
		if(state==State::PLAIN){
			uint8_t block = bitops::read_n(in_i,block_size,texture);
			if(block==0) state=State::RLE;
			else{
				for(uint8_t _i=0;_i<block_size;_i++){
					const uint8_t color =
						delta_context_helper(block & (1 << block_size-_i-1))
						? color1 : color2;
					if(color!=TRANSPARENT)
						display.drawPixel(_x+(out_i+_i)%width,_y+(out_i+_i)/width,color);
				}
				display.display();
				out_i+=block_size;
			}
			in_i+=block_size;
		}else{
			//counting to the first zero
			uint8_t packet_length = 1;
			while(true){
				if(!bitops::read_single(in_i+packet_length-1,texture)){
					// -1 because the length is one more than the last index
					break;
				}
				packet_length++;
			}
			in_i+=packet_length;
			assert(packet_length<=8);
			//extracting packet value
			uint16_t packet_value =
				(bitops::read_n(in_i,packet_length,texture)//reading pre-truncated word (since the first bit is obviously a 1, it's been truncated)
				+(1 << packet_length)//adding missing 1 at the beginning of the word
				-1)//subtracting one, so it's possible to encode 1
				*block_size;//times block size, because that's what it encodes
			//drawing if not everything is transparent
			if(mode==Mode::R && color2==TRANSPARENT){
				;//delta-decoding isn't on, so it's all just transparent
			}else if(mode==Mode::ER && color2==TRANSPARENT){
				//delta-ENcoding: either one single pixel or nothing at all
				if(delta_flags.r1==1){
					display.drawPixel(_x+out_i%width,_y+out_i/width,color1);
					delta_flags.r1=0;
				}
			}else if(mode==Mode::DR && ((!delta_flags.r1 && color2==TRANSPARENT) || (delta_flags.r1 && color1==TRANSPARENT))){
				//more complex than I'm willing to describe in a comment, but also everything is transparent
			}else{
				for(uint16_t _i=0;_i<packet_value;_i++){
					display.drawPixel(
						_x+(out_i+_i)%width,
						_y+(out_i+_i)/width,
						delta_context_helper(0) ? color1 : color2
					);
				}
				display.display();
			}
			in_i+=packet_length;
			out_i+=packet_value*block_size;
			state=State::PLAIN;
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
