#ifndef LEVELA_H
#define LEVELA_H

#include "Scene.h"

class LevelA : public Scene {
private:
    static constexpr int LEVEL_WIDTH = 14;
    static constexpr int LEVEL_HEIGHT = 8;

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
         22,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  22,
         22,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  22,
         22,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  22,
         22,  0,  0,  0,  0,  0,  0,  0,  22,  0,  0,  0,  0,  22,
         22,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  22,
        22,  0,  0,  22,  0,  0,  0,  0,  0,  0,  0,  0,  0, 22, 
        22, 22, 0, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
        42, 42, 0, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42 
    };

public:
    static constexpr float TILE_DIMENSION = 75.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD = 800.0f;

    LevelA();
    LevelA(Vector2 origin, const char *bgHexCode);
    ~LevelA();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif