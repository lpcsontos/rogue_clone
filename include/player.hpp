#pragma once

#include <vector>
#include "item.hpp"

class Player{
    private:
		int curr_depth;
		int power;
      int hp;
      int level;
      int xp;
      int def;

      int x,y;

      std::vector<Item> inventory;
      int inv_cap;

    public:
      Player();
      Player(int _curr_depth, int _power, int _hp, int _level, int _xp, int _def, int _inv_cap, int _x, int _y);
		int getDepth();
		int getAttack();
		void setX(int _x);
		void setY(int _y);
};
