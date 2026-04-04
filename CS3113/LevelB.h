#ifndef LevelB_H
#define LevelB_H

#include "Scene.h"

class LevelB : public Scene {
private:
    static constexpr int LEVEL_WIDTH = 14;
    static constexpr int LEVEL_HEIGHT = 8; // Level A is 8 rows tall
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        142,   0,   0,   0,   0,   0,   0,   0,  21,  23,   0,   0,   0, 142, // Stone Ceiling
        142,   0,   0,   0,   0,   0,   0,  21,  0, 0,   0,   0,   0, 142, // Top escape platform
        142,   0,   0,   0,   0,  21,  22,   0, 0, 0, 142,   0,   0, 142, // Mid-right platform
        142,   22,   0,   0,   0,   0,   0,   0, 0, 0, 142,   0,   0, 142, 
        142,   0,  21,  22,   0,   0,   0,   42, 42, 0, 142,   0,   0, 142, // Mid-left platform
        142,   0,   0,   0,   0,   0,   0,   0, 0, 0, 142,   0,   0, 142, 
        142,   0,   0,   0,   0,  21, 0,  0, 0, 0, 142,   0,   0, 142, // Bottom jump platform
        142,  22,  22,  22,  22,  42,  42,  42, 42, 42, 142,   0,   0, 142  // Floor & Central Pillar
    };

public:
    static constexpr float TILE_DIMENSION = 75.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD = 800.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif