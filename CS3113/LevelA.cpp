#include "LevelA.h"
extern Sound sfxDie; 
extern int gLives; 
LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() { shutdown(); }
void LevelA::initialise()
{
   mGameState.nextSceneID = -1;

   // ----------- Map -----------
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   
      (unsigned int *) mLevelData, 
      "assets/tilemap_packed.png",        
      TILE_DIMENSION,              
      20, 9,                       
      mOrigin                      
   );

   // ----------- Player -----------
   //Atlas for player
   std::map<Direction, std::vector<int>> playerAnimationAtlas = {
      {DOWN,  { 0 }},                   
      {UP,    { 4 }},                   
      {RIGHT, { 2,4,8,9 }},          
      {LEFT,  { 2,4,8,9}}           
   };

   mGameState.player = new Entity(
      {mOrigin.x + 350.0f, mOrigin.y + 150.0f}, 
      {100.0f, 130.0f},                         
      "assets/Yellow.png",                        
      ATLAS,                                    
      {2,6},                                 
      playerAnimationAtlas,                     
      PLAYER                                    
   );

   mGameState.player->setJumpingPower(650.0f);
   mGameState.player->setColliderDimensions({
      mGameState.player->getScale().x / 1.5f,
      mGameState.player->getScale().y / 1.2f
   });
   mGameState.player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

  // ----------- Enemy -----------
   mGameState.enemyCount = 1;
   mGameState.enemies = new Entity*[mGameState.enemyCount];

   //Atlas for enemy
   std::map<Direction, std::vector<int>> enemyAnimationAtlas = {
      {DOWN,  { 0 }},
      {UP,    { 7 }},
      {RIGHT, { 3, 6, 7, 10 }}, 
      {LEFT,  { 3,6,7,10}}
   };

   //Creating a new Entity
   mGameState.enemies[0] = new Entity(
      {mOrigin.x - 250.0f, mOrigin.y - 150.0f}, 
      {100.0f, 130.0f},
      "assets/Green.png",                 
      ATLAS,
      { 3, 5 },                               // 3x5
      enemyAnimationAtlas,
      NPC 
   );

   //To make the AI model walk around
   mGameState.enemies[0]->setAIType(WANDERER);
   mGameState.enemies[0]->setAIState(WALKING); 
   mGameState.enemies[0]->setSpeed(200); 
   mGameState.enemies[0]->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
}

void LevelA::update(float deltaTime)
{
   mGameState.player->update(deltaTime, nullptr, mGameState.map, nullptr, 0);

   for (int i = 0; i < mGameState.enemyCount; i++) {
      
      //Updating the enemy
      mGameState.enemies[i]->update(
         deltaTime, 
         mGameState.player, 
         mGameState.map, 
         nullptr, 
         0
      );

      // Collision 
      if (mGameState.player->isColliding(mGameState.enemies[i])) {
         PlaySound(sfxDie); 
         
         // Reset
         mGameState.enemies[i]->setPosition({mOrigin.x - 250.0f, mOrigin.y - 150.0f}); 
         mGameState.enemies[i]->resetMovement(); 
         mGameState.enemies[i]->setAIState(WALKING); 

         // Update
         gLives--;

         if (gLives <= 0) {
               mGameState.nextSceneID = 5; // Lose
         } else {
               mGameState.player->setPosition({mOrigin.x + 350.0f, mOrigin.y + 150.0f});
               mGameState.player->resetMovement();
         }
      }
   }
   
   float playerY = mGameState.player->getPosition().y;
   // Go next if player falls into the hole
   if (playerY > mOrigin.y + 600.0f) {
         mGameState.nextSceneID = 2; //Level B
   }
}
    
void LevelA::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.map->render(); 
   mGameState.player->render();

   for (int i = 0; i < mGameState.enemyCount; i++) {
      mGameState.enemies[i]->render(); 
   }
}

void LevelA::shutdown()
{
   delete mGameState.player;
   delete mGameState.map;
   
   for (int i = 0; i < mGameState.enemyCount; i++) {
      delete mGameState.enemies[i];
   }
   
   delete[] mGameState.enemies; 
}