#include "LevelB.h"
extern Sound sfxDie; 
extern int gLives; 

LevelB::LevelB()                                      : Scene { {0.0f}, nullptr   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelB::~LevelB() { shutdown(); }
void LevelB::initialise()
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
   //Atlas for Player
   std::map<Direction, std::vector<int>> playerAnimationAtlas = {
      {DOWN,  { 0 }},                   
      {UP,    { 4 }},                   
      {RIGHT, { 2,4,8,9 }},          
      {LEFT,  { 2,4,8,9}}              
   };

   mGameState.player = new Entity(
      {mOrigin.x - 375.0f, mOrigin.y + 150.0f}, 
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

   // Atlas for Enemy
   std::map<Direction, std::vector<int>> enemyAnimationAtlas = {
      {DOWN,  { 4 }},
      {UP,    { 8 }},
      {RIGHT, { 3, 6, 7, 9 }}, 
      {LEFT,  { 3,6,7,9 }}
   };

   mGameState.enemies[0] = new Entity(
      {mOrigin.x - 400.0f, mOrigin.y - 150.0f}, 
      {100.0f, 130.0f},
      "assets/Gray.png",                 
      ATLAS,
      { 3,5 },                                // 3x5
      enemyAnimationAtlas,
      NPC 
   );

   //Set the AI model to Follower
   mGameState.enemies[0]->setAIType(FOLLOWER); 
   mGameState.enemies[0]->setAIState(IDLE);
   mGameState.enemies[0]->setSpeed(125); 
   mGameState.enemies[0]->setColliderDimensions({
      mGameState.enemies[0]->getScale().x / 1.5f,
      mGameState.enemies[0]->getScale().y / 1.2f
   });
   mGameState.enemies[0]->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
}
void LevelB::update(float deltaTime)
{
   mGameState.player->update(
      deltaTime,      
      nullptr,        
      mGameState.map, 
      nullptr,        
      0               
   );
   
   for (int i = 0; i < mGameState.enemyCount; i++) {
      mGameState.enemies[i]->update(
         deltaTime, 
         mGameState.player, // Follower AI needs the player's position
         mGameState.map, 
         nullptr, 
         0
      );

      if (mGameState.player->isColliding(mGameState.enemies[i])) {
         PlaySound(sfxDie); 

         // Reset
         mGameState.enemies[i]->setPosition({mOrigin.x - 400.0f, mOrigin.y - 150.0f}); 
         mGameState.enemies[i]->resetMovement(); 
         mGameState.enemies[i]->setAIState(IDLE); 
         //Update lives
         gLives--;

         if (gLives <= 0) {
               // Lose
               mGameState.nextSceneID = 5; 
         } else {
               //Reset
               mGameState.player->setPosition({mOrigin.x - 375.0f, mOrigin.y + 150.0f});
               mGameState.player->resetMovement();
         }
      }
   }

   if (mGameState.player->getPosition().y > mOrigin.y + 600.0f) {
      mGameState.nextSceneID = 3; //Go to LevelC
   }
}
    
void LevelB::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.map->render(); 
   mGameState.player->render();

   for (int i = 0; i < mGameState.enemyCount; i++) {
       mGameState.enemies[i]->render(); 
   }
}

void LevelB::shutdown()
{
   delete mGameState.player;
   delete mGameState.map;
   
   for (int i = 0; i < mGameState.enemyCount; i++) {
       delete mGameState.enemies[i];
   }

   delete[] mGameState.enemies; 
}