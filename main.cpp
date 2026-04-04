/**
* Author: Zhi Chen
* Assignment: Rise of the AI
* Date due: 2026-04-04, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/MenuLevel.h"
#include "CS3113/WinLevel.h"
#include "CS3113/LoseLevel.h"

// Global Constants
constexpr int SCREEN_WIDTH    = 1000,
              SCREEN_HEIGHT   = 600,
              FPS             = 120;

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Camera2D gCamera = { 0 };
Scene *gCurrentScene = nullptr;

Scene* scenes[6]; 
int currentSceneIndex = 0;

Sound sfxJump;
Sound sfxDie;
Sound sfxSteps[7];
float footstepTimer = 0.0f; 
Music gBackgroundMusic;
int gLives = 3;

// Function Prototypes
void initialise();
void processInput();
void update();
void render();
void shutdown();

int main(void) {
    initialise();
    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }
    shutdown();
    return 0;
}

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Creatures");
    InitAudioDevice();

    gBackgroundMusic = LoadMusicStream("assets/audio/Adventure Meme.mp3");
    gBackgroundMusic.looping = true;
    PlayMusicStream(gBackgroundMusic);
    SetMusicVolume(gBackgroundMusic, 0.1f);

    sfxDie = LoadSound("assets/audio/gruntsound.wav");
    sfxJump = LoadSound("assets/audio/jump.wav");
    
    // Footsteps
    sfxSteps[0] = LoadSound("assets/audio/stepdirt_2.wav");
    sfxSteps[1] = LoadSound("assets/audio/stepdirt_3.wav");
    sfxSteps[2] = LoadSound("assets/audio/stepdirt_4.wav");
    sfxSteps[3] = LoadSound("assets/audio/stepdirt_5.wav");
    sfxSteps[4] = LoadSound("assets/audio/stepdirt_6.wav");
    sfxSteps[5] = LoadSound("assets/audio/stepdirt_7.wav");
    sfxSteps[6] = LoadSound("assets/audio/stepdirt_8.wav");

    for (int i = 0; i < 7; i++) SetSoundVolume(sfxSteps[i], 0.75f);
    SetSoundVolume(sfxJump, 0.2f);

    //All 6 scene
    scenes[0] = new MenuLevel(ORIGIN, "#2c3e50"); 
    scenes[1] = new LevelA(ORIGIN, "#d69f96"); 
    scenes[2] = new LevelB(ORIGIN, "#9dbbb1"); 
    scenes[3] = new LevelC(ORIGIN, "#b19dbb");
    scenes[4] = new WinLevel(ORIGIN, "#27ae60");
    scenes[5] = new LoseLevel(ORIGIN, "#c0392b");

    currentSceneIndex = 0;
    gCurrentScene = scenes[currentSceneIndex];
    gCurrentScene->initialise();

    if (gCurrentScene->getState().player != nullptr) {
        gCamera.target = gCurrentScene->getState().player->getPosition();
    } else {
        gCamera.target = ORIGIN; //Reset to origin 
    }
    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput() {
    if (gCurrentScene->getState().player != nullptr) {
        gCurrentScene->getState().player->resetMovement();

        if      (IsKeyDown(KEY_A)) gCurrentScene->getState().player->moveLeft();
        else if (IsKeyDown(KEY_D)) gCurrentScene->getState().player->moveRight();

        if (IsKeyPressed(KEY_W) && gCurrentScene->getState().player->isCollidingBottom()) {
            gCurrentScene->getState().player->jump();
            PlaySound(sfxJump); 
        }
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
    if (IsKeyPressed(KEY_ONE))   gCurrentScene->setNextSceneID(1);
    if (IsKeyPressed(KEY_TWO))   gCurrentScene->setNextSceneID(2);
    if (IsKeyPressed(KEY_THREE)) gCurrentScene->setNextSceneID(3);
}

void update() {
    UpdateMusicStream(gBackgroundMusic); 

    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP) {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        if (gCurrentScene->getState().player != nullptr) {
            Vector2 currentPlayerPosition = gCurrentScene->getState().player->getPosition();
            panCamera(&gCamera, &currentPlayerPosition);
        }
    }

    // Switch Scene
    if (gCurrentScene->getState().nextSceneID != -1) {
        int nextID = gCurrentScene->getState().nextSceneID;
        
        if (nextID == 0) gLives = 3; // If menu, then it reset lives

        gCurrentScene->shutdown();
        currentSceneIndex = nextID;
        gCurrentScene = scenes[currentSceneIndex];
        gCurrentScene->initialise();
    }

    if (gCurrentScene->getState().player != nullptr) {
        Entity* player = gCurrentScene->getState().player;
        if (player->isCollidingBottom() && (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))) {
            footstepTimer += FIXED_TIMESTEP; 
            if (footstepTimer >= 0.3f) { 
                PlaySound(sfxSteps[GetRandomValue(0, 6)]);
                footstepTimer = 0.0f;
            }
        }
    }
}

void render()
{
    BeginDrawing();
    if (gCurrentScene->getState().player != nullptr) {
        BeginMode2D(gCamera);
            gCurrentScene->render();
        EndMode2D();
    } else {
        gCurrentScene->render();
    }

    DrawText(TextFormat("LIVES: %i", gLives), 20, 20, 40, MAROON);
    EndDrawing(); 
}
void shutdown() {
    UnloadMusicStream(gBackgroundMusic);
    UnloadSound(sfxJump);
    UnloadSound(sfxDie);
    for (int i = 0; i < 7; i++) UnloadSound(sfxSteps[i]);

    for (int i = 0; i < 6; i++) {
        if (scenes[i] != nullptr) {
            delete scenes[i];
            scenes[i] = nullptr;
        }
    }

    CloseAudioDevice();
    CloseWindow();
}