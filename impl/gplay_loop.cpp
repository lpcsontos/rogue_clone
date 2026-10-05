#include "graphics.hpp"

State s = MENU;

void game(Map m, int w, int h){

	switch(s){
		case MENU:
			drawMenu(w, h, s);
			break;
		case GAMEPLAY:
			drawLevel(m, w, h);
			break;
		case SETTINGS:
			drawSettings(w, h, s);
			break;
	}
}


