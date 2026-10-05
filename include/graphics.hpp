#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "map.hpp"
#include "raylib.h"
#include "raygui.h"

enum State{
	MENU,
	GAMEPLAY,
	SETTINGS
};

void drawMenu(int w, int h, State& s);
void drawLevel(Map m, int w, int h);
void drawSettings(int w, int h, State& s);

void game(Map m, int w, int h);
