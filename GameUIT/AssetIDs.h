#pragma once

//
// This file contains all IDs of game assets 
//

#pragma region ID_EACH_SCENE

#define ID_INTRO	5
#define ID_MAP_1_1	1
#define ID_WORLD	2
#define ID_WORLD_SUB 3 //World dự phòng để Load lại game

#pragma endregion

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_MISC_2 30

#pragma region MAP1-1

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBAS	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_COIN	4
#define OBJECT_TYPE_PLATFORM 5
#define OBJECT_TYPE_COLOR_PLATFORM 6
#define OBJECT_TYPE_QUESTION_BRICK 7
#define OBJECT_TYPE_CARD	8
#define OBJECT_TYPE_TUBE	9
#define OBJECT_TYPE_PORTAL	50

#pragma endregion

#pragma region WORLD MAP

#define OBJECT_TYPE_MARIO_AT_WORLD_MAP	100
#define OBJECT_TYPE_GRASS	110
#define OBJECT_TYPE_ENTRANCE	120
#define OBJECT_TYPE_BLOCK_POINT	130
#define OBJECT_TYPE_GATE_KEEPER	140

#pragma endregion

#pragma region INTRO

#define OBJECT_TYPE_INTRO_LAYER	800
#define OBJECT_TYPE_RED_CURTAIN	900
#define OBJECT_TYPE_MARIO_INTRO	1000
#define OBJECT_TYPE_LUIGI_INTRO	1100
#define OBJECT_TYPE_SMB3_CURTAIN	1200
#define OBJECT_TYPE_NUMBER_3_RACOON	1300

#pragma endregion

#pragma region BRICK 

#define ID_SPRITE_STRIPE_BRICK 19000
#define ID_SPRITE_BRICK 20000
#define ID_SPRITE_QBRICK 20500

#pragma endregion 


#pragma region COIN

#define ID_SPRITE_COIN 40000

#pragma endregion 

#pragma region MUSHROOM

#define ID_SPRITE_MUSHROOM_IN_BRICK 45000
#define ID_SPRITE_MUSHROOM_OUT_BRICK 45316

#pragma endregion 

#pragma region LEAF

#define ID_SPRITE_LEAF_RIGHT 60000
#define ID_SPRITE_LEAF_LEFT 60500

#pragma endregion 

#pragma region SHOOTING FLOWER

#define ID_SPRITE_FLOWER_RISE_UP 80000
#define ID_SPRITE_FLOWER_IDLE 81000

#pragma endregion 

#define ID_SPRITE_GROUND 400000

#define ID_SPRITE_TUBE 5400

#define ID_SPRITE_EFFECT_SCORE_100 100
#define ID_SPRITE_EFFECT_SCORE_2O0	200
#define ID_SPRITE_EFFECT_SCORE_4O0	400