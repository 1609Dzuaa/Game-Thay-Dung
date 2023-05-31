/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102

	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work, applying on Mario, Brick, Goomba & Coin

	Key functions:
		CCollision::SweptAABB
		CCollision::SweptAABBEx
		CCollision::Scan
		CCollision::Filter
		CCollision::Process

		CGameObject::GetBoundingBox

================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <list>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Animation.h"
#include "Animations.h"

#include "Mario.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Mushroom.h"
#include "Coin.h"
#include "Platform.h"
#include "Leaf.h"

#include "SampleKeyEventHandler.h"

#include "AssetIDs.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240


#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario_trans.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc_transparent.png"
#define TEXTURE_PATH_MISC_2 TEXTURES_DIR "\\misc_2.png"
#define TEXTURE_PATH_ENEMY TEXTURES_DIR "\\enemies_transparent.png"
#define TEXTURE_PATH_BBOX TEXTURES_DIR "\\bbox.png"

CGame* game;
CMario* mario;

list<LPGAMEOBJECT> objects;

CSampleKeyHandler* keyHandler;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void LoadAssetsMario()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// IDLE
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1, 1028, 92, 1042, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1, 1, 92, 15, 119, texMario);

	// WALKING RIGHT
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 2, 1009, 92, 1024, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 3, 990, 92, 1007, 119, texMario);

	// WALKING LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 2, 18, 92, 34, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 3, 37, 92, 53, 119, texMario);

	// RUNNING RIGHT 
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 1, 933, 92, 953, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 2, 908, 92, 927, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 3, 882, 92, 901, 119, texMario);

	// RUNNING LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 1, 90, 92, 110, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 2, 117, 92, 136, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 3, 143, 92, 162, 119, texMario);

	// JUMP WALK RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT + 1, 954, 92, 971, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT + 1, 72, 92, 89, 119, texMario);

	// JUMP RUN RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT + 1, 856, 92, 875, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT + 1, 169, 92, 188, 119, texMario);

	// SIT RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_SIT_RIGHT + 1, 974, 92, 989, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_SIT_LEFT + 1, 55, 92, 69, 119, texMario);

	// BRACING RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_BRACE_RIGHT + 1, 832, 92, 849, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_BRACE_LEFT + 1, 195, 92, 210, 119, texMario);

	// KICKING RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_KICK_RIGHT + 1, 717, 92, 737, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_KICK_LEFT + 1, 305, 92, 326, 119, texMario);

	sprites->Add(ID_SPRITE_MARIO_BIG_HOLDING_LEFT + 1, 266, 92, 282, 119, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_HOLDING_LEFT + 2, 284, 92, 301, 119, texMario);

	sprites->Add(ID_SPRITE_MARIO_BIG_HOLDING_RIGHT + 1, 470, 191, 498, 219, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_HOLDING_RIGHT + 2, 499, 192, 527, 219, texMario);

	// MARIO DIE
	sprites->Add(ID_SPRITE_MARIO_DIE + 1, 306, 15, 322, 32, texMario);

	// SMALL MARIO 
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1, 1029, 16, 1040, 32, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1, 2, 16, 15, 32, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 2, 1009, 16, 1024, 32, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 2, 19, 16, 35, 32, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 1, 973, 16, 989, 32, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 2, 955, 16, 971, 32, texMario);
	//sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 3, 335, 0, 335 + 15, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 1, 54, 16, 71, 32, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 2, 73, 16, 88, 32, texMario);
	//sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 3, 95, 0, 95 + 15, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_BRACE_LEFT + 1, 110, 16, 124, 32, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_BRACE_RIGHT + 1, 920, 16, 934, 32, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_KICK_LEFT + 1, 181, 15, 197, 31, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_KICK_RIGHT + 1, 846, 15, 863, 31, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT + 1, 37, 16, 53, 32, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT + 1, 991, 16, 1007, 32, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT + 1, 90, 16, 107, 32, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT + 1, 936, 16, 953, 32, texMario);

	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_RUNNING_RIGHT, ani);

	// Mario runs faster hence animation speed should be faster
	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_KICK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_KICKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_KICK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_KICKING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_SIT_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SIT_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_SIT_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SIT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_DIE + 1);
	animations->Add(ID_ANI_MARIO_DIE, ani);

	//
	// SMALL MARIO 
	//

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 2);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 2);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_RACOON_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 2);
	animations->Add(ID_ANI_MARIO_SMALL_RUNNING_RIGHT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 2);
	animations->Add(ID_ANI_MARIO_SMALL_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_KICK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_KICKING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_KICK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_KICKING_RIGHT, ani);

	//
	// RACOON MARIO 
	//
	
	sprites->Add(ID_SPRITE_MARIO_RACOON_IDLE_LEFT + 1, 1, 349, 22, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_WALKING_LEFT + 2, 27, 349, 48, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_WALKING_LEFT + 3, 52, 349, 76, 377, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_IDLE_RIGHT + 1, 1021, 349, 1042, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_WALKING_RIGHT + 2, 995, 349, 1016, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_WALKING_RIGHT + 3, 968, 349, 991, 377, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_RUNNING_LEFT + 1, 183, 349, 207, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_RUNNING_LEFT + 2, 209, 349, 233, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_RUNNING_LEFT + 3, 235, 349, 259, 377, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_RUNNING_RIGHT + 1, 836, 349, 861, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_RUNNING_RIGHT + 2, 810, 349, 835, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_RUNNING_RIGHT + 3, 784, 349, 808, 377, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_BRACE_LEFT + 1, 339, 347, 355, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_BRACE_RIGHT + 1, 688, 347, 705, 377, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_JUMP_WALK_LEFT + 1, 260, 349, 285, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_JUMP_WALK_RIGHT + 1, 758, 349, 783, 377, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_JUMP_RUN_LEFT + 1, 60, 713, 85, 741, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_JUMP_RUN_RIGHT + 1, 360, 713, 385, 741, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_SIT_LEFT + 1, 79, 359, 101, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_SIT_RIGHT + 1, 943, 359, 965, 377, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_KICK_LEFT + 1, 81, 383, 103, 411, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_KICK_RIGHT + 1, 940, 383, 963, 411, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 1, 375, 349, 398, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 2, 357, 349, 373, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 3, 645, 349, 668, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 4, 400, 349, 417, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 5, 375, 349, 398, 377, texMario);

	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 1, 646, 349, 669, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 2, 670, 349, 687, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 3, 375, 349, 398, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 4, 400, 349, 417, 377, texMario);
	sprites->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 5, 646, 349, 669, 377, texMario);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_RACOON_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_RACOON_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_RACOON_WALKING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_RACOON_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_RACOON_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_RACOON_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_RACOON_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_RACOON_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_RACOON_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_RACOON_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_RACOON_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_RACOON_RUNNING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_SIT_LEFT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_SIT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_SIT_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_SIT_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_KICK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_KICKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_RACOON_KICK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_RACOON_KICKING_LEFT, ani);

	ani = new CAnimation(70);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 3);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 4);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_LEFT + 5);
	animations->Add(ID_ANI_MARIO_RACOON_ATTACKING_LEFT, ani);

	ani = new CAnimation(70);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 3);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 4);
	ani->Add(ID_SPRITE_MARIO_RACOON_ATTACK_RIGHT + 5);
	animations->Add(ID_ANI_MARIO_RACOON_ATTACKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_HOLDING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_HOLDING_LEFT + 2);
	animations->Add(ID_ANI_MARIO_BIG_HOLDING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_HOLDING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_HOLDING_RIGHT + 2);
	animations->Add(ID_ANI_MARIO_BIG_HOLDING_RIGHT, ani);
}

void LoadAssetsGoomba()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);

	sprites->Add(ID_SPRITE_GOOMBA_WALK + 1, 31, 348, 31 + 17, 348 + 16, texEnemy);
	sprites->Add(ID_SPRITE_GOOMBA_WALK + 2, 53, 348, 53 + 15, 348 + 17, texEnemy);

	sprites->Add(ID_SPRITE_GOOMBA_DIE + 1, 11, 355, 11 + 15, 355 + 9, texEnemy);
	sprites->Add(ID_SPRITE_GOOMBA_DIE_REVERSE + 1, 170, 349, 170 + 16, 349 + 16, texEnemy);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_WALK + 1);
	ani->Add(ID_SPRITE_GOOMBA_WALK + 2);
	animations->Add(ID_ANI_GOOMBA_WALKING, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_DIE + 1);
	animations->Add(ID_ANI_GOOMBA_DIE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_DIE_REVERSE + 1);
	animations->Add(ID_ANI_GOOMBA_DIE_REVERSE, ani);
}
void LoadAssetsKoopa()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);

	sprites->Add(ID_SPRITE_KOOPA_WALK_LEFT + 1, 70, 65, 70 + 17, 65 + 27, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_WALK_LEFT + 2, 88, 66, 88 + 15, 66 + 26, texEnemy);

	sprites->Add(ID_SPRITE_KOOPA_WALK_RIGHT + 1, 655, 66, 655 + 16, 66 + 26, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_WALK_RIGHT + 2, 638, 66, 638 + 15, 66 + 26, texEnemy);

	sprites->Add(ID_SPRITE_KOOPA_SLEEP + 1, 70, 123, 70 + 16, 123 + 15, texEnemy);

	sprites->Add(ID_SPRITE_KOOPA_SLEEP_REVERSE + 1, 210, 348, 227, 364, texEnemy);

	sprites->Add(ID_SPRITE_KOOPA_SLIP + 1, 105, 123, 105 + 15, 123 + 15, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_SLIP + 2, 122, 123, 122 + 15, 123 + 15, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_SLIP + 3, 70, 123, 70 + 16, 123 + 15, texEnemy);

	sprites->Add(ID_SPRITE_KOOPA_SLIP_REVERSE + 1, 252, 349, 268, 365, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_SLIP_REVERSE + 2, 269, 349, 285, 365, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_SLIP_REVERSE + 3, 286, 349, 302, 365, texEnemy);

	sprites->Add(ID_SPRITE_KOOPA_REBORN + 1, 49, 138, 49 + 17, 138 + 16, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_REBORN + 1, 49, 138, 49 + 17, 138 + 16, texEnemy);

	sprites->Add(ID_SPRITE_KOOPA_DIE + 1, 210, 348, 210 + 17, 348 + 16, texEnemy);

	//KOOPA JUMPING LEFT
	sprites->Add(ID_SPRITE_KOOPA_JUMPING_LEFT + 1, 70, 94, 87, 121, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_JUMPING_LEFT + 2, 121, 94, 138, 121, texEnemy);

	//KOOPA JUMPING RIGHT
	sprites->Add(ID_SPRITE_KOOPA_JUMPING_RIGHT + 1, 655, 94, 670, 121, texEnemy);
	sprites->Add(ID_SPRITE_KOOPA_JUMPING_RIGHT + 2, 603, 94, 619, 121, texEnemy);

	//KOOPA REBORN
	sprites->Add(ID_SPRITE_KOOPA_REBORN + 1, 105, 75, 122, 92, texEnemy);

	//KOOPA REBORN REVERSE
	sprites->Add(ID_SPRITE_KOOPA_REBORN_REVERSE + 1, 230, 347, 248, 365, texEnemy);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_JUMPING_LEFT + 1);
	ani->Add(ID_SPRITE_KOOPA_JUMPING_LEFT + 2);
	animations->Add(ID_ANI_KOOPA_JUMPING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_JUMPING_RIGHT + 1);
	ani->Add(ID_SPRITE_KOOPA_JUMPING_RIGHT + 2);
	animations->Add(ID_ANI_KOOPA_JUMPING_RIGHT, ani);

    ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_WALK_LEFT + 1);
	ani->Add(ID_SPRITE_KOOPA_WALK_LEFT + 2);
	animations->Add(ID_ANI_KOOPA_WALKING_LEFT, ani);

    ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_WALK_RIGHT + 1);
	ani->Add(ID_SPRITE_KOOPA_WALK_RIGHT + 2);
	animations->Add(ID_ANI_KOOPA_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_SLEEP + 1);
	animations->Add(ID_ANI_KOOPA_SLEEPING, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_SLEEP_REVERSE + 1);
	animations->Add(ID_ANI_KOOPA_SLEEPING_REVERSE, ani);

	ani = new CAnimation(45);
	ani->Add(ID_SPRITE_KOOPA_SLEEP + 1);
	ani->Add(ID_SPRITE_KOOPA_SLIP + 1);
	ani->Add(ID_SPRITE_KOOPA_SLIP + 2);
	ani->Add(ID_SPRITE_KOOPA_SLIP + 3);
	animations->Add(ID_ANI_KOOPA_SLIPPING, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_DIE + 1);
	animations->Add(ID_ANI_KOOPA_DIE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_SLEEP + 1);
	ani->Add(ID_SPRITE_KOOPA_REBORN + 1);
	animations->Add(ID_ANI_KOOPA_REBORN, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_SLEEP_REVERSE + 1);
	ani->Add(ID_SPRITE_KOOPA_REBORN_REVERSE + 1);
	animations->Add(ID_ANI_KOOPA_REBORN_REVERSE, ani);

	ani = new CAnimation(45);
	ani->Add(ID_SPRITE_KOOPA_SLEEP_REVERSE + 1);
	ani->Add(ID_SPRITE_KOOPA_SLIP_REVERSE + 1);
	ani->Add(ID_SPRITE_KOOPA_SLIP_REVERSE + 2);
	ani->Add(ID_SPRITE_KOOPA_SLIP_REVERSE + 3);
	animations->Add(ID_ANI_KOOPA_SLIPPING_REVERSE, ani);
}
void LoadAssetsBrick()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_BRICK + 1, 372, 153, 372 + 15, 153 + 15, texMisc);

	texMisc = textures->Get(ID_TEX_MISC_2);
	sprites->Add(ID_SPRITE_QBRICK + 1, 157, 188, 172, 203, texMisc);
	sprites->Add(ID_SPRITE_QBRICK + 2, 176, 188, 191, 203, texMisc);
	sprites->Add(ID_SPRITE_QBRICK + 3, 196, 188, 211, 203, texMisc);
	sprites->Add(ID_SPRITE_QBRICK + 4, 215, 188, 230, 203, texMisc);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BRICK + 1);
	animations->Add(ID_ANI_BRICK, ani);

	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_QBRICK + 1);
	ani->Add(ID_SPRITE_QBRICK + 2);
	ani->Add(ID_SPRITE_QBRICK + 3);
	ani->Add(ID_SPRITE_QBRICK + 4);
	animations->Add(ID_ANI_QUESTION_BRICK, ani);

	//QBrick -> Normal Brick
	sprites->Add(ID_SPRITE_QBRICK + 5, 235, 188, 250, 203, texMisc);
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_QBRICK + 5);
	animations->Add(ID_ANI_QUESTION_BRICK_HITTED, ani);
}
void LoadAssetsCoin()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);

	sprites->Add(ID_SPRITE_COIN + 1, 303, 99, 303 + 9, 99 + 15, texMisc);
	sprites->Add(ID_SPRITE_COIN + 2, 321, 99, 321 + 9, 99 + 15, texMisc);
	sprites->Add(ID_SPRITE_COIN + 3, 338, 99, 338 + 9, 99 + 15, texMisc);

	LPANIMATION ani = new CAnimation(150);
	ani->Add(ID_SPRITE_COIN + 1);
	ani->Add(ID_SPRITE_COIN + 2);
	ani->Add(ID_SPRITE_COIN + 3);
	animations->Add(ID_ANI_COIN, ani);
}

void LoadAssetMushroom()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC_2);

	sprites->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 1, 162, 386, 174, 388, texMisc);
	sprites->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 2, 182, 384, 196, 388, texMisc);
	sprites->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 3, 203, 382, 219, 388, texMisc);
	sprites->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 4, 225, 380, 241, 388, texMisc);
	sprites->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 5, 246, 376, 262, 388, texMisc);
	sprites->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 6, 268, 373, 284, 388, texMisc);

	sprites->Add(ID_SPRITE_MUSHROOM_OUT_BRICK + 1, 156, 121, 172, 137, texMisc);

	LPANIMATION ani = new CAnimation(135);
	ani->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 1);
	ani->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 2);
	ani->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 3);
	ani->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 4);
	ani->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 5);
	ani->Add(ID_SPRITE_MUSHROOM_IN_BRICK + 6);
	animations->Add(ID_ANI_MUSHROOM_IN_BRICK, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MUSHROOM_OUT_BRICK + 1);
	animations->Add(ID_ANI_MUSHROOM_OUT_BRICK, ani);
}

void LoadAssetsOther()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_CLOUD_BEGIN, 390, 117, 390 + 15, 117 + 15, texMisc);
	sprites->Add(ID_SPRITE_CLOUD_MIDDLE, 408, 117, 408 + 15, 117 + 15, texMisc);
	sprites->Add(ID_SPRITE_CLOUD_END, 426, 117, 426 + 15, 117 + 15, texMisc);

	texMisc = textures->Get(ID_TEX_MISC_2);
	sprites->Add(ID_SPRITE_LEAF_LEFT + 1, 180, 123, 195, 137, texMisc);
	sprites->Add(ID_SPRITE_LEAF_RIGHT + 1, 160, 362, 176, 376, texMisc);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_LEAF_LEFT + 1);
	animations->Add(ID_ANI_LEAF_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_LEAF_RIGHT + 1);
	animations->Add(ID_ANI_LEAF_RIGHT, ani);
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_ENEMY, TEXTURE_PATH_ENEMY);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_MISC_2, TEXTURE_PATH_MISC_2);
	textures->Add(ID_TEX_BBOX, TEXTURE_PATH_BBOX);

	LoadAssetsMario();
	LoadAssetsGoomba();
	LoadAssetsKoopa();
	LoadAssetMushroom();
	LoadAssetsBrick();
	LoadAssetsCoin();
	LoadAssetsOther();
}

void ClearScene()
{
	list<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

#define MARIO_START_X 20.0f
#define MARIO_START_Y 10.0f

#define BRICK_X 0.0f
#define GOOMBA_X 200.0f
#define KOOPA_X 270.0f
#define COIN_X 100.0f

#define BRICK_Y GROUND_Y + 20.0f
#define NUM_BRICKS 70

/*
* Reload all objects of current scene
* NOTE: super bad way to build a scene! We need to load a scene from data instead of hard-coding like this
*/
void Reload()
{
	ClearScene();

	// Main ground
	for (int i = 0; i < NUM_BRICKS; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y);
		objects.push_back(b);
	}

	// Short, low platform
	for (int i = 1; i < 3; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y - 44.0f);
		objects.push_back(b);
	}

	// Vertical column 1
	for (int i = 0; i < 10; i++)
	{
		CBrick* b = new CBrick(0, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}

	// Vertical column 2
	for (int i = 1; i < 3; i++)
	{
		CBrick* b = new CBrick(BRICK_X + 300.0f, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}

	// Vertical column 3
	for (int i = 1; i < 4; i++)
	{
		CBrick* b = new CBrick(BRICK_X + 400.0f, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}

	// Vertical column 4
	for (int i = 1; i < 5; i++)
	{
		CBrick* b = new CBrick(BRICK_X + 500.0f, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}

	for (int i = 0; i < 2; i++)
	{
		CQuestionBrick* qb = new CQuestionBrick(BRICK_X + 200.0f + i * 16.0f, 125.0f, HAS_MUSHROOM);
		objects.push_back(qb);
	}

	// Second cloud platform 
	CPlatform* p = new CPlatform(90.0f, GROUND_Y - 74.0f,
		16, 15, 16, ID_SPRITE_CLOUD_BEGIN, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_END);
	objects.push_back(p);

	mario = new CMario(MARIO_START_X, MARIO_START_Y);
	objects.push_back(mario);

	/*for (int j = 0; j < 1; j++)
	{
		CGoomba* goomba = new CGoomba(GOOMBA_X + j * 60, GROUND_Y - 120.0f);
		objects.push_back(goomba);
	}

	for (int j = 0; j < 1; j++)
	{
		CKoopa* koopa = new CKoopa(KOOPA_X + j * 60, GROUND_Y - 120.0f, 2);
		objects.push_back(koopa);
	}*/

	/*for (int j = 0; j < 1; j++)
	{
		CLeaf* leaf = new CLeaf(KOOPA_X + j * 60, GROUND_Y - 120.0f);
		objects.push_back(leaf);
	}*/

	// COINS 
	for (int i = 0; i < 10; i++)
	{
		CCoin* c = new CCoin(COIN_X + i * (COIN_WIDTH * 2), GROUND_Y - 96.0f);
		objects.push_back(c);
	}
}

bool IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void PurgeDeletedObjects()
{
	list<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), IsGameObjectDeleted),
		objects.end());
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	list<LPGAMEOBJECT>::iterator i;
	for (i = objects.begin(); i != objects.end(); ++i)
	{
		coObjects.push_back(*i);
	}

	for (i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->Update(dt, &coObjects);
	}

	PurgeDeletedObjects();

	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	cy = 0;
	//cy -= SCREEN_HEIGHT / 2;

	if (cx < 0) cx = 0;

	CGame::GetInstance()->SetCamPos(cx, cy);
}

/*
	Render a frame
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	list<LPGAMEOBJECT>::iterator i;
	for (i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->Render();
	}

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = (DWORD)(now - frameStart);

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetDebugWindow(hWnd);

	game = CGame::GetInstance();
	game->Init(hWnd, hInstance);

	keyHandler = new CSampleKeyHandler();
	game->InitKeyboard(keyHandler);

	LoadResources();
	Reload();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}