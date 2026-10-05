#include <string>
#include "enemy.hpp"

constexpr int HEIGHT = 50;
constexpr int WIDTH = 200;

enum tileType{
    WALL=0,
    FLOOR,
    CHEST,
    STAIR,
    VOID
};

struct Room{
    uint32_t x,y;
    int w,h;
};

class Level{
    private:
      tileType map[HEIGHT][WIDTH];
      std::vector<Room> rooms;
		std::vector<Enemy> enemies;

    public:
      Level(){}
      tileType atCoord(int y, int x){return map[y][x];}
      void setCoord(int y, int x, tileType type) {map[y][x] = type;}

      std::vector<Room> getRooms(){ return rooms;}
      void addRoom(Room r){ rooms.push_back(r);}

		std::vector<Enemy> getEnemies(){return enemies;}
		void addEnemy(Enemy entity){enemies.push_back(entity);}
};
