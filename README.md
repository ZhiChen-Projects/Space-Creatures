# Space Creatures
Project 4 for NYU CS-UY 3113

Using Raylib Library, programed in C++ to create a platformer from the bottom-up, meaning I made my own 2D Engine.

This project was specifcally to target the AI Models we have in today's games. 

By AI Models, I do not mean actual machine learning, but rather NPCS that does it's own action.
These are the AI Models that I implemented in my game:
- WANDER
- FOLLOWER
- LARPER

Wander: Pretty straight forward but it's a model that wanders around and does it's own thing.
Follower: It follows the player model.
Larper: Accelerate towards the player, but drastially decrease until it gets near the player.

Now that we understand the 3 models, here's more information about this game.
The players have 3 lives, and everytime the enemy model touches you, you lose a life and you restart at that level.
By level, there 3 levels to this game, LevelA, LevelB, LevelC.  
LevelA uses the WANDER model, LevelB uses the FOLLOWER model, and LevelC uses the LARPER Model. LevelA automatically activate the AI, so players will be caught off guard with it. Both LevelB and LevelC is that the enemy only gets close to you once you enter it's radius, which is an implementation done with some basic math. However, I purposely made the radius small on LevelB so players can find a little easy path to get to the next level. LevelC is a bit more challenging since the path is directly blocked by the enemy, and the player must figure out how to avoid them.

This project's theme was Space with the free assets that I found on [Kenny](https://kenney.nl/assets).
