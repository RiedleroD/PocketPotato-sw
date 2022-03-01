#define SETTINGS_LAST_INDEX 0

void showSettings(){
	int curItem = 0;
	uint8_t new_brightness=EEPROM.read(ADR_BRIGHTNESS);
	bool isSelected = false;
	while(true){
		if (up.isClick() || up.isStep()){
			if(isSelected){
				if(curItem==0)
					++new_brightness;
			}else if(curItem<SETTINGS_LAST_INDEX){
				++curItem;
			}
		}else if (down.isClick() || down.isStep()){
			if(isSelected){
				if(curItem==0)
					--new_brightness;
			}else if(curItem<SETTINGS_LAST_INDEX){
				--curItem;
			}
		}else if(right.isClick() || r_sh.isClick()){
			isSelected=!isSelected;
		}else if (left.isClick() || l_sh.isClick()){
			if(isSelected)
				isSelected=false;
			else{
				if(new_brightness!=EEPROM.read(ADR_BRIGHTNESS)){
					EEPROM.write(ADR_BRIGHTNESS,new_brightness);
				}
				loadSettings();
				return;
			}
		}
		display.clearDisplay();
		display.setCursor(10,0);
		display.print(F("Brightness"));
		display.setCursor(100,0);
		display.print(new_brightness);
		drawTexture(isSelected? 90:1,curItem*10,arrow,5,9);
		display.display();
	}
}

void loadSettings(){
	//TODO: dim
}