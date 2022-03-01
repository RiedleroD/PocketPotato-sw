/**
 * This programm is to test if the buttons work correctly.
 */

void testButtons() {
	while (true) {
		if (down.isClick() || down.isStep())
			display.print("D");
		else if (up.isClick() || up.isStep())
			display.print("U");
		else if (left.isClick())
			display.print("L");
		else if (right.isClick())
			display.print("R");
		else if (sh_r.isClick())
			display.print("RS");
		else if (sh_l.isClick())
			display.print("LS");
		display.display();
		
		if(up.isHold() && down.isHold()) {
			return;
		}
	}
}
