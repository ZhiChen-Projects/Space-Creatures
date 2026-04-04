#ifndef LevelC_H
#define LevelC_H

#include "Scene.h"

class LevelC : public Scene {
private:
    static constexpr int LEVEL_WIDTH = 14;
    static constexpr int LEVEL_HEIGHT = 11; 

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0,  0,  0,  0,  0,  0,144,  0,  0,  0,  0,  0,  0,  144,
        0,  0,  0,  0,  0,  0,0,  0,  0,  0,  0,  0,  0,  144,
         144,  0,  0,  0,  0,  0,144,  0,  0,  0,  0,  0,  0,  144,
         142,  0,  0,  0,  0,  0,144,  0,  0,  0,  0,  0,  0,  142,
         143,  0,  0,  0,  0,144,  0,  0,142,142,142,  0,  0,  143,
         144,  0,  0,  0,142,  0,143,  0,  0,  0,  0,  0,  0,  144,
         143,  0,144,143,  0,  0,  0,  0,  0,  0,  0,143,  0,  143,
         142,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,142,  0,  142,
        22, 22,  0, 0, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 
        42, 42, 0, 0, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 
        42, 42, 0, 0, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42  
    };

public:
    static constexpr float TILE_DIMENSION = 75.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD = 800.0f;

    LevelC();
    LevelC(Vector2 origin, const char *bgHexCode);
    ~LevelC();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif