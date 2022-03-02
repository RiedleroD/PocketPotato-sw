void eepromtest() {
	uint8_t num;

	EEPROM.get(ADR_TESTSCORE, num);

	while(true) {
		if(up.isClick())
			num += 1;
		if(down.isClick())
			num -= 1;
		if(right.isClick())
			EEPROM.put(ADR_TESTSCORE, num);
		if(left.isClick())
			return;

		display.clearDisplay();

		display.setCursor(0,0);
		display.print(num);

		display.display();
	}
}
