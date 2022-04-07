namespace pong{
	MENUSTRINGS4(speed,"Slowest","Slow","Normal","Fast");
	
	int16_t calculateBallSpeed(const uint8_t paddle_size,const uint8_t by,const uint8_t y1,const uint8_t ball_speed){
		return (ball_speed/5)*(255/paddle_size)*(1+by-y1-(paddle_size/2));
	}
	
	//paddle_size has to be divisible by 2.
	//ball_speed is between 1 (slowest) and 10 (fastest)
	void game(const uint8_t paddle_size,const uint8_t ball_speed){
		//paddle positions
		uint8_t y1 = 31-paddle_size/2;
		uint8_t y2 = y1;
		
		uint16_t ballCoords[2] = {63 << 8,31 << 8};//actual coords times 2⁸
		int16_t ballSpeeds[2] = {pow(ball_speed,2)*2,0};//255 is one pixel per tick
		ballSpeeds[1]=ballSpeeds[0];
		
		uint32_t t1;
		uint16_t score[2] = {0,0};
		
		while(true){
			t1 = millis();
		/*rendering*/
			display.clearDisplay();
			//drawing paddles
			for(uint8_t _y=0;_y<paddle_size;++_y){
				for(uint8_t _x=0;_x<2;++_x){
					display.drawPixel(_x,y1+_y,SSD1306_WHITE);
					display.drawPixel(126+_x,y2+_y,SSD1306_WHITE);
				}
			}
			//drawing ball
			for(uint8_t _y=0;_y<2;++_y)
				for(uint8_t _x=0;_x<2;++_x)
					display.drawPixel(
						(ballCoords[0] >> 8) + _x,
						(ballCoords[1] >> 8) + _y,
						SSD1306_WHITE
					);
			//drawing score as 0:0 in the center of the screen
			display.setCursor(score[0]>9 ? 55-6*(uint8_t)(log10(score[0])) : 55,0);
			display.print(score[0]);
			display.print(':');
			display.print(score[1]);
			//flipping display
			display.display();
		/*input handling*/
			if(up.state() && y1>0)
				y1--;
			if(down.state() && y1<(64-paddle_size))
				y1++;
			if(left.state() && y2>0)
				y2--;
			if(right.state() && y2<(64-paddle_size))
				y2++;
		/*ball handling 😏*/
		{
			uint8_t bx=ballCoords[0] >> 8;
			uint8_t by=ballCoords[1] >> 8;
			//changing vertical direction on wall bounce
			if((by<=0 && ballSpeeds[1]<0) || (by>=63 && ballSpeeds[1]>0))
				ballSpeeds[1]=-ballSpeeds[1];
			//goal handling
			if(bx-1 >= 125){
				ballCoords[0] = 63 << 8;
				ballCoords[1] = 31 << 8;
				ballSpeeds[0] = pow(ball_speed,2)*2;
				if(bx==0 || bx==255){
					score[0]++;
				}else if(bx>=124){
					score[1]++;
					ballSpeeds[0]=-ballSpeeds[0];//sending ball to previous victor
				}
			//changing horizontal & vertical direction on paddle bounce + slight speedup
			}else if(bx<2 && ballSpeeds[0]<0 && (by+1)>=y1 && by<=(y1+paddle_size)){
				ballSpeeds[0]=-1.02*ballSpeeds[0];
				ballSpeeds[1]=calculateBallSpeed(paddle_size,by,y1,ball_speed);
			}else if(bx>=124 && ballSpeeds[0]>0 && (by+1)>=y2 && by<=(y2+paddle_size)){
				ballSpeeds[0]=-1.02*ballSpeeds[0];
				ballSpeeds[1]=calculateBallSpeed(paddle_size,by,y2,ball_speed);
			}
			//advancing positions
			ballCoords[0] += ballSpeeds[0];
			ballCoords[1] += ballSpeeds[1];
		}
		/*speed limiting*/
			while(millis()-t1 < 10);
		}
	}
	void run(){
		game(10,1+3*menu::draw(speeds,speedcount));
	}
}