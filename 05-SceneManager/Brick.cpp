#include "Brick.h"
#include "AssetIDs.h"
#define BRICK_BOUNDING_X 5.0f
#define BROKEN_DISTANCE_X	8.0f
CBrick::CBrick(float x, float y, float brickType, int itemType) :CGameObject(x, y)
{
	this->type = OBJECT_TYPE_BRICK;
	this->brickType = brickType;
	this->start_x = x;
	this->start_y = y;
	this->itemType = itemType;
	SetState(BRICK_STATE_ACTIVE);
}
void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (brokenPieces.size() > 0) {
		for (int i = 0; i < brokenPieces.size(); i++)
			brokenPieces[i]->Render();
		return;
	}
	int ani = 0;
	if (brickType != BRICK_TYPE_HIDDEN) {
	if (brickType == BRICK_TYPE_QUESTION) {
		ani = ID_ANI_QUESTION_BRICK_ACTIVE;
		if (state == BRICK_STATE_EMPTY) {
			ani = ID_ANI_QUESTION_BRICK_EMPTY;
		}
		else ani = ID_ANI_QUESTION_BRICK_ACTIVE;
		animations->Get(ani)->Render(x, y);
	}
	else if (brickType == BRICK_TYPE_GOLD)
	{
		ani = ID_ANI_GOLD_BRICK_ACTIVE;
		animations->Get(ani)->Render(x, y);
	}
	}
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}
void CBrick::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//CGameObject::Update(dt);
	if ((state == BRICK_STATE_BROKEN) && (GetTickCount64() - brickBroken_start > 400))
	{
		isDeleted = true;
		return;
	}
	if (brokenPieces.size() > 0) { // state broken
		if (brokenPieces.size() == 4) {
			for (int i = 0; i < brokenPieces.size(); i++)
				brokenPieces[i]->Update(dt, coObjects);
			//piece->Update(dt, coObjects);
		}

	}
	
	if (state == BRICK_STATE_BOUND) {
		if (y < (start_y - BRICK_BOUNDING_X) && vy < 0) {
			vy = -vy;
		}
		if (y > start_y) {
			y = start_y;
			SetState(BRICK_STATE_EMPTY);
		}
	}
	y += vy * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{


}
void CBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state) {
	case BRICK_STATE_BOUND:
	{
		if (itemType == CONTAIN_MUSHROOM) {
			isFallingItem = true;
		}
		else
			isFallingItem = false;
		vy = -0.1f;
	}
	break;

	case BRICK_STATE_ACTIVE:
		vy = 0;
		break;
	case BRICK_STATE_EMPTY:
		vy = 0;
		break;
	case BRICK_STATE_BROKEN:
	{
		
		brickBroken_start = GetTickCount64();
		isBroken = true;
		CBrickBrokenPieces* piece1 = new CBrickBrokenPieces(start_x, start_y, 1, 2.2f); //PHAI DUOI
		CBrickBrokenPieces* piece2 = new CBrickBrokenPieces(start_x + BROKEN_DISTANCE_X, start_y + BROKEN_DISTANCE_X, 1, 1.0f); // PHAI TREN
		CBrickBrokenPieces* piece3 = new CBrickBrokenPieces(start_x, start_y, -1, 2.2f); // TRAI DUOI
		CBrickBrokenPieces* piece4 = new CBrickBrokenPieces(start_x, start_y + BROKEN_DISTANCE_X, -1, 1.0f); // TRAI TREN
		brokenPieces = { piece1, piece2, piece3, piece4 };
	}
	break;
	}

}