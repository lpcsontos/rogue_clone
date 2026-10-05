#include <bits/stdc++.h>
//#include "include/map.hpp"
#include "include/graphics.hpp"

int main(){
   Map d(123, 1);

	const int scW = 800;
	const int scH = 600;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(scW, scH, "test");
	SetTargetFPS(60);

	while(!WindowShouldClose()){
		BeginDrawing();

		//drawLevel(d, scW, scH);
		game( d, GetScreenWidth(), GetScreenHeight());

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
