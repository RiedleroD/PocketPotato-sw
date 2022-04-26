namespace pong{
	MENUSTRINGS5(menu,"Start: PvP","       PvE","       PvW","Score","Close");
	MENUSTRINGS4(speed,"Slowest","Slow","Normal","Fast");
	MENUSTRINGS5(paddlesize,"Pixel","Stick","Paddle","Pipe","Tower");
	
	int16_t calculateBallSpeed(const uint8_t paddle_size,const uint8_t by,const uint8_t y1,const uint8_t ball_speed){
		return (ball_speed/5)*(255/paddle_size)*(1+by-y1-(paddle_size/2));
	}
	
	inline void resetBall(const uint8_t mode,const uint8_t ball_speed,uint16_t ballCoords[2],int16_t ballSpeeds[2]){
		ballCoords[1] = 31 << 8;
		if(mode==2){
			ballCoords[0] = 120 << 8;
			ballSpeeds[0] = -pow(ball_speed,2)*2;
		}else{
			ballCoords[0] = 63 << 8;
			ballSpeeds[0] = pow(ball_speed,2)*2;
		}
	}
	
	//paddle_size has to be divisible by 2.
	//ball_speed is between 1 (slowest) and 10 (fastest)
	void game(const uint8_t paddle_size,const uint8_t ball_speed,const uint8_t mode){
		//paddle positions
		uint8_t y1 = 31-paddle_size/2;
		uint8_t y2 = y1;
		
		uint16_t ballCoords[2] = {0,0};//actual coords times 2⁸
		int16_t ballSpeeds[2] = {0,0};//255 is one pixel per tick
		resetBall(mode,ball_speed,ballCoords,ballSpeeds);
		ballSpeeds[1]=ballSpeeds[0];
		
		//invert text so ball can still be seen behind it
		display.setTextColor(INVERT);
		
		uint32_t t1;
		uint16_t score[2] = {0,0};
		
		while(true){
			t1 = millis();
		/*rendering*/
			display.clearDisplay();
			//drawing paddles
			fillRect(0,y1,2,paddle_size,WHITE);
			if(mode==2)//drawing the wall in highscore mode instead
				fillRect(126,0,2,64,WHITE);
			else
				fillRect(126,y2,2,paddle_size,WHITE);
			//drawing ball
			fillRect(
				ballCoords[0] >> 8,
				ballCoords[1] >> 8,
				2,2,WHITE);
			//drawing score as 0:0 in the center of the screen
			//or as 0 if playing highscore
			
			if(mode==2){
				display.setCursor(0,0);
				display.print(score[0]);
			}else{
				display.setCursor(score[0]>9 ? 55-6*(uint8_t)(log10(score[0])) : 55,0);
				display.print(score[0]);
				display.print(':');
				display.print(score[1]);
			}
			//flipping display
			display.display();
		/*input handling*/
			if(up.state() && y1>0)
				y1--;
			if(down.state() && y1<(64-paddle_size))
				y1++;
			if(mode==0){//second player handling
				if(left.state() && y2>0)
					y2--;
				if(right.state() && y2<(64-paddle_size))
					y2++;
			}
			if(sh_l.isClick())
				return;
		/*paddle AI in PvE mode*/
		if(mode==1){
			if(ballSpeeds[0]>0 && (ballCoords[0]>>8)>64){//ball goes right & is in the right half of the field
				const uint8_t by=ballCoords[1]>>8;
				if(ballSpeeds[1]<0 && by<(y2+paddle_size) && y2>0)//ball goes up & is above paddle bottom
					y2--;
				else if(ballSpeeds[1]>0 && by>y2 && y2<(64-paddle_size))//ball goes down & is below paddle top
					y2++;
				else if(ballSpeeds[1]==0){
					const uint8_t midpoint=y2+paddle_size/2;
					if(by<midpoint && y2>0)
						y2--;
					else if(by>midpoint && y2<(64-paddle_size))
						y2++;
				}
			}
		}
		/*ball handling 😏*/
		{
			uint8_t bx=ballCoords[0] >> 8;
			uint8_t by=ballCoords[1] >> 8;
			//changing vertical direction on wall bounce
			if((by<=0 && ballSpeeds[1]<0) || (by>=63 && ballSpeeds[1]>0))
				ballSpeeds[1]=-ballSpeeds[1];
			//goal handling
			if(bx-1 >= 125){
				resetBall(mode,ball_speed,ballCoords,ballSpeeds);
				if(mode==2){
					//checking if new highscore
					uint16_t hiscore;
					EEPROM.get(ADR_PONGSCORE,hiscore);
					if(hiscore<score[0]){
						display.setCursor(34,32);//centered text
						display.setTextColor(WHITE,BLACK);//overwrites background
						display.print(F("HIGHSCORE!"));
						display.display();
						EEPROM.put(ADR_PONGSCORE,score[0]);
						display.setTextColor(INVERT);//reset text color
						_delay_ms(1000);
					}
					//resetting score
					score[0] = 0;
				}else{
					if(bx==0 || bx==255){//goal into left
						score[1]++;
					}else if(bx>=124){//goal into right
						score[0]++;
						ballSpeeds[0]=-ballSpeeds[0];//sending ball to previous victor
					}
				}
			//changing horizontal & vertical direction on paddle bounce + slight speedup
			}else if(bx<2 && ballSpeeds[0]<0 && (by+1)>=y1 && by<=(y1+paddle_size)){
				ballSpeeds[0]=-1.02*ballSpeeds[0];
				ballSpeeds[1]=calculateBallSpeed(paddle_size,by,y1,ball_speed);
			}else if(bx>=124 && ballSpeeds[0]>0){
				if(mode==2){
					ballSpeeds[0]=-1.02*ballSpeeds[0];
					score[0]+=1;
				}else if((by+1)>=y2 && by<=(y2+paddle_size)){
					ballSpeeds[1]=calculateBallSpeed(paddle_size,by,y2,ball_speed);
					ballSpeeds[0]=-1.02*ballSpeeds[0];
				}
			}
			//advancing positions
			ballCoords[0] += ballSpeeds[0];
			ballCoords[1] += ballSpeeds[1];
		}
		/*speed limiting*/
			while(millis()-t1 < 10);
		}
	}
	void showScore(){
		display.clearDisplay();
		display.setCursor(10,27);
		display.print(F("Highscore: "));
		{
			const uint16_t hiscore;
			EEPROM.get(ADR_PONGSCORE,hiscore);
			display.print(hiscore);
		}
		display.display();
		while(true){
			if(left.isClick() || sh_l.isClick())
				break;
		}
	}
	void run(){
		uint8_t mode;
		while(true){
			mode = menu::draw(menus,menucount);
			switch(mode){
				case 0:
				case 1:
				case 2:
					game(
						pow(2, 1+menu::draw(paddlesizes,paddlesizecount)),
						1 + 3*menu::draw(speeds,speedcount),
						mode
					);
					break;
				case 3:
					showScore();
					break;
				case 4:
					return;
			}
		}
	}
}