#pragma once

//
// This file contains all IDs of game assets 
//

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_MISC_2 30


#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBAS	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_COIN	4
#define OBJECT_TYPE_PLATFORM 5
#define OBJECT_TYPE_PORTAL	50


#pragma region MARIO 

#define ID_SPRITE_MARIO 10000
#define ID_SPRITE_MARIO_BIG (ID_SPRITE_MARIO + 1000)
#define ID_SPRITE_MARIO_BIG_IDLE (ID_SPRITE_MARIO_BIG + 100)
#define ID_SPRITE_MARIO_BIG_IDLE_LEFT (ID_SPRITE_MARIO_BIG_IDLE + 10)
#define ID_SPRITE_MARIO_BIG_IDLE_RIGHT (ID_SPRITE_MARIO_BIG_IDLE + 20)

#define ID_SPRITE_MARIO_BIG_WALKING (ID_SPRITE_MARIO_BIG + 200)
#define ID_SPRITE_MARIO_BIG_WALKING_LEFT (ID_SPRITE_MARIO_BIG_WALKING + 10)
#define ID_SPRITE_MARIO_BIG_WALKING_RIGHT (ID_SPRITE_MARIO_BIG_WALKING + 20)
#define ID_SPRITE_MARIO_BIG_RUNNING (ID_SPRITE_MARIO_BIG + 300)
#define ID_SPRITE_MARIO_BIG_RUNNING_LEFT (ID_SPRITE_MARIO_BIG_RUNNING + 10)
#define ID_SPRITE_MARIO_BIG_RUNNING_RIGHT (ID_SPRITE_MARIO_BIG_RUNNING + 20)
#define ID_SPRITE_MARIO_BIG_JUMP (ID_SPRITE_MARIO_BIG + 400)
#define ID_SPRITE_MARIO_BIG_JUMP_WALK (ID_SPRITE_MARIO_BIG_JUMP + 10)
#define ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 2)
#define ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 6)
#define ID_SPRITE_MARIO_BIG_JUMP_RUN (ID_SPRITE_MARIO_BIG_JUMP + 20)
#define ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT (ID_SPRITE_MARIO_BIG_JUMP_RUN + 2)
#define ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_BIG_JUMP_RUN + 6)
#define ID_SPRITE_MARIO_BIG_SIT (ID_SPRITE_MARIO_BIG + 500)
#define ID_SPRITE_MARIO_BIG_SIT_LEFT (ID_SPRITE_MARIO_BIG_SIT + 10)
#define ID_SPRITE_MARIO_BIG_SIT_RIGHT (ID_SPRITE_MARIO_BIG_SIT + 20)
#define ID_SPRITE_MARIO_BIG_BRACE (ID_SPRITE_MARIO_BIG + 600)
#define ID_SPRITE_MARIO_BIG_BRACE_LEFT (ID_SPRITE_MARIO_BIG_BRACE + 10)
#define ID_SPRITE_MARIO_BIG_BRACE_RIGHT (ID_SPRITE_MARIO_BIG_BRACE + 20)
#define ID_SPRITE_MARIO_BIG_KICK (ID_SPRITE_MARIO_BIG + 700)
#define ID_SPRITE_MARIO_BIG_KICK_LEFT (ID_SPRITE_MARIO_BIG_KICK + 10) 
#define ID_SPRITE_MARIO_BIG_KICK_RIGHT (ID_SPRITE_MARIO_BIG_KICK + 20)
#define ID_SPRITE_MARIO_BIG_HOLDING (ID_SPRITE_MARIO_BIG + 800)
#define ID_SPRITE_MARIO_BIG_HOLDING_LEFT (ID_SPRITE_MARIO_BIG_HOLDING + 10) 
#define ID_SPRITE_MARIO_BIG_HOLDING_RIGHT (ID_SPRITE_MARIO_BIG_HOLDING + 20)

#define ID_SPRITE_MARIO_DIE (ID_SPRITE_MARIO + 3000)

#define ID_SPRITE_MARIO_SMALL (ID_SPRITE_MARIO + 2000)
#define ID_SPRITE_MARIO_SMALL_IDLE (ID_SPRITE_MARIO_SMALL + 100)
#define ID_SPRITE_MARIO_SMALL_IDLE_LEFT (ID_SPRITE_MARIO_SMALL_IDLE + 10)
#define ID_SPRITE_MARIO_SMALL_IDLE_RIGHT (ID_SPRITE_MARIO_SMALL_IDLE + 20)

#define ID_SPRITE_MARIO_SMALL_WALKING (ID_SPRITE_MARIO_SMALL + 200)
#define ID_SPRITE_MARIO_SMALL_WALKING_LEFT (ID_SPRITE_MARIO_SMALL_WALKING + 10)
#define ID_SPRITE_MARIO_SMALL_WALKING_RIGHT (ID_SPRITE_MARIO_SMALL_WALKING + 20)
#define ID_SPRITE_MARIO_SMALL_RUNNING (ID_SPRITE_MARIO_SMALL + 300)
#define ID_SPRITE_MARIO_SMALL_RUNNING_LEFT (ID_SPRITE_MARIO_SMALL_RUNNING + 10)
#define ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT (ID_SPRITE_MARIO_SMALL_RUNNING + 20)
#define ID_SPRITE_MARIO_SMALL_JUMP (ID_SPRITE_MARIO_SMALL + 400)
#define ID_SPRITE_MARIO_SMALL_JUMP_WALK (ID_SPRITE_MARIO_SMALL_JUMP + 10)
#define ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT (ID_SPRITE_MARIO_SMALL_JUMP_WALK + 2)
#define ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP_WALK + 6)
#define ID_SPRITE_MARIO_SMALL_JUMP_RUN (ID_SPRITE_MARIO_SMALL_JUMP + 20)
#define ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT (ID_SPRITE_MARIO_SMALL_JUMP_RUN + 2)
#define ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP_RUN + 6)
#define ID_SPRITE_MARIO_SMALL_KICK_LEFT (ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 15)
#define ID_SPRITE_MARIO_SMALL_KICK_RIGHT (ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 15)


#define ID_SPRITE_MARIO_SMALL_BRACE (ID_SPRITE_MARIO_SMALL + 500)
#define ID_SPRITE_MARIO_SMALL_BRACE_LEFT (ID_SPRITE_MARIO_SMALL_BRACE + 10)
#define ID_SPRITE_MARIO_SMALL_BRACE_RIGHT (ID_SPRITE_MARIO_SMALL_BRACE + 20)

//
//RACOON MARIO
//

#define ID_SPRITE_MARIO_RACOON (ID_SPRITE_MARIO + 4000)
#define ID_SPRITE_MARIO_RACOON_IDLE (ID_SPRITE_MARIO_RACOON + 100)
#define ID_SPRITE_MARIO_RACOON_IDLE_LEFT (ID_SPRITE_MARIO_RACOON_IDLE + 10)
#define ID_SPRITE_MARIO_RACOON_IDLE_RIGHT (ID_SPRITE_MARIO_RACOON_IDLE + 20)
#define ID_SPRITE_MARIO_RACOON_WALKING (ID_SPRITE_MARIO_RACOON + 200)
#define ID_SPRITE_MARIO_RACOON_WALKING_LEFT (ID_SPRITE_MARIO_RACOON_WALKING + 10)
#define ID_SPRITE_MARIO_RACOON_WALKING_RIGHT (ID_SPRITE_MARIO_RACOON_WALKING + 20)
#define ID_SPRITE_MARIO_RACOON_RUNNING (ID_SPRITE_MARIO_RACOON + 300)
#define ID_SPRITE_MARIO_RACOON_RUNNING_LEFT (ID_SPRITE_MARIO_RACOON_RUNNING + 10)
#define ID_SPRITE_MARIO_RACOON_RUNNING_RIGHT (ID_SPRITE_MARIO_RACOON_RUNNING + 20)
#define ID_SPRITE_MARIO_RACOON_JUMP (ID_SPRITE_MARIO_RACOON + 400)
#define ID_SPRITE_MARIO_RACOON_JUMP_WALK (ID_SPRITE_MARIO_RACOON_JUMP + 10)
#define ID_SPRITE_MARIO_RACOON_JUMP_WALK_LEFT (ID_SPRITE_MARIO_RACOON_JUMP_WALK + 2)
#define ID_SPRITE_MARIO_RACOON_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_RACOON_JUMP_WALK + 6)
#define ID_SPRITE_MARIO_RACOON_JUMP_RUN (ID_SPRITE_MARIO_RACOON_JUMP + 20)
#define ID_SPRITE_MARIO_RACOON_JUMP_RUN_LEFT (ID_SPRITE_MARIO_RACOON_JUMP_RUN + 2)
#define ID_SPRITE_MARIO_RACOON_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_RACOON_JUMP_RUN + 6)

#define ID_SPRITE_MARIO_RACOON_BRACE (ID_SPRITE_MARIO_RACOON + 500)
#define ID_SPRITE_MARIO_RACOON_BRACE_LEFT (ID_SPRITE_MARIO_RACOON_BRACE + 10)
#define ID_SPRITE_MARIO_RACOON_BRACE_RIGHT (ID_SPRITE_MARIO_RACOON_BRACE + 20)

#define ID_SPRITE_MARIO_RACOON_SIT (ID_SPRITE_MARIO_RACOON + 600)
#define ID_SPRITE_MARIO_RACOON_SIT_LEFT (ID_SPRITE_MARIO_RACOON_SIT + 10)
#define ID_SPRITE_MARIO_RACOON_SIT_RIGHT (ID_SPRITE_MARIO_RACOON_SIT + 20)
#define ID_SPRITE_MARIO_RACOON_KICK (ID_SPRITE_MARIO_RACOON + 700)
#define ID_SPRITE_MARIO_RACOON_KICK_LEFT (ID_SPRITE_MARIO_RACOON_KICK + 10)
#define ID_SPRITE_MARIO_RACOON_KICK_RIGHT (ID_SPRITE_MARIO_RACOON_KICK + 20)
#define ID_SPRITE_MARIO_RACOON_ATTACK (ID_SPRITE_MARIO_RACOON + 800)
#define ID_SPRITE_MARIO_RACOON_ATTACK_LEFT (ID_SPRITE_MARIO_RACOON_ATTACK + 10)
#define ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT (ID_SPRITE_MARIO_RACOON_ATTACK + 20)

#pragma endregion 


#pragma region BRICK 

#define ID_SPRITE_BRICK 20000
#define ID_SPRITE_QBRICK 20500

#pragma endregion 


#pragma region GOOMBA & PARA_GOOMBA

#define ID_SPRITE_GOOMBA 30000
#define ID_SPRITE_GOOMBA_WALK (ID_SPRITE_GOOMBA + 1000)
#define ID_SPRITE_GOOMBA_DIE (ID_SPRITE_GOOMBA + 2000)
#define ID_SPRITE_GOOMBA_DIE_REVERSE (ID_SPRITE_GOOMBA + 2001)

#define ID_SPRITE_PARA_GOOMBA 33000
#define ID_SPRITE_PARA_GOOMBA_WALK (ID_SPRITE_PARA_GOOMBA + 100)
#define ID_SPRITE_PARA_GOOMBA_DIE (ID_SPRITE_PARA_GOOMBA + 200)
#define ID_SPRITE_PARA_GOOMBA_DIE_REVERSE (ID_SPRITE_PARA_GOOMBA + 201)
#define ID_SPRITE_PARA_GOOMBA_FLY (ID_SPRITE_PARA_GOOMBA + 300)
#define ID_SPRITE_PARA_GOOMBA_CALM (ID_SPRITE_PARA_GOOMBA + 400)

#pragma endregion 


#pragma region KOOPA

#define ID_SPRITE_KOOPA 35000
#define ID_SPRITE_KOOPA_WALK_LEFT (ID_SPRITE_KOOPA + 1000)
#define ID_SPRITE_KOOPA_WALK_RIGHT (ID_SPRITE_KOOPA + 1500)
#define ID_SPRITE_KOOPA_SLEEP (ID_SPRITE_KOOPA + 2000)
#define ID_SPRITE_KOOPA_SLEEP_REVERSE (ID_SPRITE_KOOPA + 2500)
#define ID_SPRITE_KOOPA_SLIP (ID_SPRITE_KOOPA + 3000)
#define ID_SPRITE_KOOPA_SLIP_REVERSE (ID_SPRITE_KOOPA + 3500)
#define ID_SPRITE_KOOPA_DIE (ID_SPRITE_KOOPA + 4000)
#define ID_SPRITE_KOOPA_REBORN (ID_SPRITE_KOOPA + 5000)
#define ID_SPRITE_KOOPA_REBORN_REVERSE (ID_SPRITE_KOOPA + 5500)

#pragma endregion 

#pragma region KOOPA_PARA

#define ID_SPRITE_KOOPA_PARA 70000
#define ID_SPRITE_KOOPA_JUMPING_LEFT (ID_SPRITE_KOOPA_PARA + 1000)
#define ID_SPRITE_KOOPA_JUMPING_RIGHT (ID_SPRITE_KOOPA_PARA + 1500)
#define ID_SPRITE_KOOPA_PARA_SLEEP (ID_SPRITE_KOOPA_PARA + 2000)
#define ID_SPRITE_KOOPA_PARA_SLIP (ID_SPRITE_KOOPA_PARA + 3000)
#define ID_SPRITE_KOOPA_PARA_DIE (ID_SPRITE_KOOPA_PARA + 4000)
#define ID_SPRITE_KOOPA_PARA_REBORN (ID_SPRITE_KOOPA_PARA + 5000)
#define ID_SPRITE_KOOPA_PARA_REBORN_REVERSE (ID_SPRITE_KOOPA_PARA + 6000)

#pragma endregion 

#pragma region COIN

#define ID_SPRITE_COIN 40000

#pragma endregion 

#pragma region MUSHROOM

#define ID_SPRITE_MUSHROOM_IN_BRICK 45000
#define ID_SPRITE_MUSHROOM_OUT_BRICK 45031

#pragma endregion 

#pragma region LEAF

#define ID_SPRITE_LEAF_RIGHT 60000
#define ID_SPRITE_LEAF_LEFT 60500

#pragma endregion 

#define ID_SPRITE_CLOUD 50000
#define ID_SPRITE_CLOUD_BEGIN (ID_SPRITE_CLOUD+1000)
#define ID_SPRITE_CLOUD_MIDDLE (ID_SPRITE_CLOUD+2000)
#define ID_SPRITE_CLOUD_END (ID_SPRITE_CLOUD+3000)


