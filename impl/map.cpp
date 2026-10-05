#include "map.hpp"
#include "util.hpp"
#include <random>

uint32_t Map::levelSeedgen(uint32_t masterSeed, int level){
    uint32_t s = masterSeed ^ (0x9E3779B9u * (uint32_t)level);
    s ^= s >> 16; s *= 0x7feb352du;
    s ^= s >> 15; s *= 0x846ca68bu;
    s ^= s >> 16;
    return s ? s : 1;
}


void Map::connectRooms(std::mt19937& prng, Room r1, Room r2, Level& l) {
    int startX = r1.x + r1.w / 2;
    int startY = r1.y + r1.h / 2;
    int endX = r2.x + r2.w / 2;
    int endY = r2.y + r2.h / 2;

    auto dig = [&](int x, int y) {
    	if (l.atCoord(y, x) == tileType::FLOOR) {
        	return; 
    	}

    	l.setCoord(y, x, tileType::FLOOR);

    	int dy[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    	int dx[] = {0, 0, -1, 1, -1, 1, -1, 1};
    	for (int k = 0; k < 8; ++k) {
        int ny = y + dy[k];
        int nx = x + dx[k];
        
		  if (l.atCoord(ny, nx) == tileType::VOID) {
            l.setCoord(ny, nx, tileType::WALL);
        }
    	}
	};

   int currentX = startX;
   int currentY = startY;

   if (randomRange(prng, 0, 1) == 0) {
        while (currentX != endX) {
            dig(currentX, currentY);
            currentX += (endX > currentX) ? 1 : -1;
        }
        while (currentY != endY) {
            dig(currentX, currentY);
            currentY += (endY > currentY) ? 1 : -1;
        }
    } else {
        while (currentY != endY) {
            dig(currentX, currentY);
            currentY += (endY > currentY) ? 1 : -1;
        }
        while (currentX != endX) {
            dig(currentX, currentY);
            currentX += (endX > currentX) ? 1 : -1;
        }
    }
    dig(endX, endY);
}

void Map::generateRooms(std::mt19937& prng, Level& l){
    int col = randomRange(prng, 2, 6);
    int row = 1;

    int maxW = (int)(WIDTH / col);
    int maxH = (int)(HEIGHT / row);

    while(l.getRooms().empty()){
        for(int i = 0; i < col; i++){
            row = randomRange(prng, 1, 3);
            maxH = (int)(HEIGHT / row);
            for(int j = 0; j < row; j++){
                if(randomRange(prng, 1, 6) > 1){
                    Room r;
                    r.w = randomRange(prng, 5, maxW-2);
                    r.h = randomRange(prng, 5, maxH-2);
                    r.x = randomRange(prng, i*maxW +1, i*maxW + maxW-r.w-1);
                    r.y = randomRange(prng, j*maxH +1, j*maxH + maxH-r.h-1);
                    l.addRoom(r);
                }
            }
        }
    }
}



void Map::generate(int level){
    std::mt19937 prng(levelSeedgen(Map::seed, level));
    Level l;

    generateRooms(prng, l);

    const std::vector<Room>& allRooms = l.getRooms();

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            
            tileType finalTile = tileType::VOID;

            for (const auto& r : allRooms) {
                if (x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h) {
                    
                    if (x == r.x || x == r.x + r.w - 1 || y == r.y || y == r.y + r.h - 1) {
                        if (finalTile != tileType::FLOOR) {
                            finalTile = tileType::WALL;
                        }
                    }
                    else {
                        finalTile = tileType::FLOOR;
                    }
                }
            }

            l.setCoord(y, x, finalTile);
        }
    }

    std::vector<Edge> allpEdges = edgeCalculator(allRooms);

    std::vector<Edge> roads = kruskalMST(l.getRooms().size(), allpEdges);    
    for(int i = 0; i < roads.size(); i++){
        connectRooms(prng, allRooms.at(roads.at(i).s), allRooms.at(roads.at(i).d), l);
    }

    levels.push_back(l);

    curr_level = 1;
}
