#include <vector>
#include <cstdint>
#include <iostream>
#include <random>
#include "level.hpp"

class Map{
    private:
        uint32_t seed;

        std::vector<Level> levels;
        int curr_level;

    public:
        Map(int level = 1){
            std::random_device rd;
            seed = rd();
            generate(level);
        }

        Map(uint32_t Seed, int level) : seed(Seed){
            generate(level);
        }

        uint32_t levelSeedgen(uint32_t masterSeed, int level);
        void generate(int level);
        void generateRooms(std::mt19937& prng, Level& l);
        void connectRooms(std::mt19937& prng, Room r1, Room r2, Level& l);
        

        const Level& getLevel(int level) const {
            return levels.at(level - 1);
        }

        const int getDepth() const{
            return curr_level;
        }

};
