#include "MenuLevel.h"

MenuLevel::MenuLevel(Vector2 origin, const char *bgHexCode) : Scene(origin, bgHexCode) {}
MenuLevel::~MenuLevel() { shutdown(); }

void MenuLevel::initialise() {
    mGameState.nextSceneID = -1;
    mGameState.player = nullptr; 
    mGameState.map = nullptr;    
    mGameState.enemyCount = 0; 
}

void MenuLevel::update(float deltaTime) {
    if (IsKeyPressed(KEY_ENTER)) {
        mGameState.nextSceneID = 1; //Go to LevelA
    }
}

void MenuLevel::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    
    DrawText("Space Creatures", mOrigin.x - 200, mOrigin.y - 60, 40, WHITE);
    DrawText("Press ENTER to Start", mOrigin.x - 130, mOrigin.y + 20, 20, LIGHTGRAY);
}

void MenuLevel::shutdown() {}