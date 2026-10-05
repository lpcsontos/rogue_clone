#pragma once

#include <string>


class Item{
    private:
        std::string id;

        int dmg;
        int def;
        int hp;
        int xp;

    public:
        Item(){}
        Item(int _dmg, int _def, int _hp, int _xp){
            dmg = _dmg;
            def = _def;
            hp = _hp;
            xp = _xp;
        }
};