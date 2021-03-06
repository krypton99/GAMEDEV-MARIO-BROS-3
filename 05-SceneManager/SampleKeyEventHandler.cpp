#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (!mario->GetIsOnPlatform()) {
			mario->SetVy(MARIO_RACOON_FALLING_SLOW_SPEED);
			mario->SetFallLower(true);
		} if (!mario->GetIsFlying()) {
			mario->SetState(MARIO_STATE_JUMP);
		}
		break;
	case DIK_A:
		mario->SetState(MARIO_STATE_ATTACK);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	case DIK_F:
		float x, y;
		mario->GetPosition(x, y);
		mario->GetGhostMario()->SetPosition(x, y);
		mario->GetGhostMario()->SetSpeed(0.2f*mario->GetGhostMario()->GetNx(), 0);
		if (mario->GetGhostMario()->GetHoldingShell() == true) {
			mario->GetGhostMario()->SetHoldingShell(false);
		} 
		break;
	}

}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:	 
		if (mario->GetState() == MARIO_STATE_FLY)
		{
			//mario->setAy(MARIO_GRAVITY);
			//mario->SetState(MARIO_STATE_RELEASE_FLY);
		}
		else
		{
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
			mario->setAy(MARIO_GRAVITY);
		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}

}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float vx, vy;
	mario->GetSpeed(vx, vy);
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
		if (game->IsKeyDown(DIK_S) && mario->GetLevel() == MARIO_LEVEL_RACOON && abs(vx) >= MARIO_RUNNING_SPEED && mario->GetIsOnPlatform()) {

			mario->SetState(MARIO_STATE_FLY);
		}
		
}