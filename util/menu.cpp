namespace menu{
	/**
	 * Returns 0 if exited without selecting, otherwise the index of the selected + 1
	 */
	uint8_t run(const char* const texts[], const uint8_t length) {
		uint8_t select = 0;
		bool didChange = true;
		//the maxIndex has to be calculated from a parameter as for some reason
		//arrays change their length when passed as parameters. this is the best
		//solution as i care about my sanity and i dont want to spend 5 hours
		//on this. good luck
		while(true) {
			//moving of index
			if(up.isClick() || up.isStep()){
				if(--select > length) //detects an unsigned int overflow
					select = length-1;
				didChange = true;
			}
			if(down.isClick() || down.isStep()){
				if(++select > length-1)
					select = 0;
				didChange = true;
			}
			if(right.isClick())
				return select+1;
			if(left.isClick())
				return 0;
			
			if(didChange){
				//drawing of menu
				display.clearDisplay();
				char arrayBuf[32];
				for(uint8_t index = 0; index < 6 && index + (select/6 * 6) <= length-1; index++) {
					display.setCursor(10, 10 * index);
					//adds 6 to the index for every page
					strcpy_P(arrayBuf, (char*) pgm_read_word(&(texts[index + (select/6 * 6)])));
					display.print(arrayBuf);
					//printFromFlash(texts[index + (select/6 * 6)]);
				}
				drawTexture(1, (select%6) * 10, arrow, 5, 9);
				display.display();
				didChange = false;
			}
		}
	}
}
