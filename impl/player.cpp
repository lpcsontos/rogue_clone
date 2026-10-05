#include "player.hpp"

Player::Player(){
   curr_depth = 1;
	power = 2;
	hp = 10;
   level = 1;
   xp = 0;
   def = 5;
   inv_cap = 10;
}

Player::Player(int _curr_depth, int _power, int _hp, int _level, int _xp, int _def, int _inv_cap, int _x, int _y){
   curr_depth = _curr_depth;
	power = _power;
	hp = _hp;
   level = _level;
   xp = _xp;
   def = _def;
   inv_cap = _inv_cap;
   x = _x;
   y = _y;
}

int Player::getAttack(){
	return power;
}

int Player::getDepth(){
	return curr_depth;
}

void Player::setX(int _x){
	x = _x;
}


void Player::setY(int _y){
	y = _y;
}
