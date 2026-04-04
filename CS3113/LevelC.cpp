#include "LevelC.h"
extern Sound sfxDie; 
extern int gLives; 

LevelC::LevelC()                      : Scene { {0.0f}, nullptr   } {}
LevelC::LevelC(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
{
   mGameState.nextSceneID = -1; // Reset so we don't immediately transition again!

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
      {mOrigin.x + 445.0f, mOrigin.y - 150.0f}, // Dropping in from the top right!
      {100.0f, 130.0f},                         
      "assets/Yellow.png",                        
      ATLAS,                                    
      { 2, 6 },                                 
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
      {UP,    { 10 }},
      {RIGHT, { 3, 7, 8 }}, 
      {LEFT,  { 3, 1, 8 }}
   };

   mGameState.enemies[0] = new Entity(
      {mOrigin.x - 350.0f, mOrigin.y + 100.0f}, 
      {100.0f, 130.0f},
      "assets/Pink.png",                  
      ATLAS,
      { 3, 5 },                                 // 3x5
      enemyAnimationAtlas,
      NPC 
   );

   mGameState.enemies[0]->setAIType(LERPER); // Lerper
   mGameState.enemies[0]->setAIState(IDLE);
   
   //No gravity so it can float
   mGameState.enemies[0]->setAcceleration({0.0f, 0.0f});
}
void LevelC::update(float deltaTime)
{
   mGameState.player->update(deltaTime, nullptr, mGameState.map, nullptr, 0);
   for (int i = 0; i < mGameState.enemyCount; i++) {
      mGameState.enemies[i]->update(
         deltaTime, 
         mGameState.player, 
         mGameState.map, 
         nullptr, 
         0
      );

      if (mGameState.player->isColliding(mGameState.enemies[i])) {
         PlaySound(sfxDie);
         
         //Enemy reset
         mGameState.enemies[i]->setPosition({mOrigin.x - 350.0f, mOrigin.y + 100.0f});
         mGameState.enemies[i]->resetMovement();
         mGameState.enemies[i]->setAIState(IDLE);

         gLives--;

         if (gLives <= 0) {
               // Lose
               mGameState.nextSceneID = 5; 
         } else {
               // Player reset
               mGameState.player->setPosition({mOrigin.x + 445.0f, mOrigin.y - 150.0f});
               mGameState.player->resetMovement();
         }
      }
   }

   float playerY = mGameState.player->getPosition().y;
   if (playerY > mOrigin.y + 600.0f && gLives > 0) {
      mGameState.nextSceneID = 4; // Win
   }

}

void LevelC::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   mGameState.map->render(); 
   mGameState.player->render();

   for (int i = 0; i < mGameState.enemyCount; i++) {
       mGameState.enemies[i]->render(); 
   }
}

void LevelC::shutdown()
{
   delete mGameState.player;
   delete mGameState.map;
   
   if (mGameState.enemies != nullptr) {
      for (int i = 0; i < mGameState.enemyCount; i++) {
         delete mGameState.enemies[i];
      }
      delete[] mGameState.enemies;
   }
}