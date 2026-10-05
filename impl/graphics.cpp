#include "graphics.hpp"

void drawLevel(Map m, int w, int h){
    Level level= m.getLevel(m.getDepth());
	 Color c[5] = {RED, WHITE, YELLOW, GREEN, BLACK}; 
	 const char* element[] = {"#", ".", "C", "Σ", " "};
	 
	 int a = w/WIDTH;
	 int b = h/HEIGHT;

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
        		DrawRectangle(j*a, i*b, a, b,c[level.atCoord(i,j)]);
		  }
    }
}



void drawMenu(int w, int h, State& s){
	ClearBackground(WHITE);

	GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(h*0.05f));
	
	Rectangle playB = {(int)(2*w/3)/2 , 30, (int)w/3, (int)h/5};
	Rectangle settB = {(int)(2*w/3)/2 , 30 + (int)h/3, (int)w/3, (int)h/5};
	Rectangle quitB = {(int)(2*w/3)/2 , 30 + (int)2*h/3, (int)w/3, (int)h/5};
	

	if(GuiButton(playB,"Play")){
		s = GAMEPLAY;	
		GuiLoadStyleDefault();
	}
	if(GuiButton(settB, "Settings")){
		s = SETTINGS;
	}
	if(GuiButton(quitB,"Quit")){
		CloseWindow();
	}
}

void drawSettings(int w, int h, State& s){
	
}


