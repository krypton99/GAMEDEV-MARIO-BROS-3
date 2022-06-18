#include "Pswitch.h"
#include "GameObject.h"
#include "Mario.h"
#include "Grid.h"
#include "Brick.h"
#include "Coin.h"
#define BRICK_BOUNDING_X 5.0f
CPswitch::CPswitch(float x, float y)
{
	this->x = x;
	this->y = y;
	state = SWITCH_STATE_ON;
	this->type = OBJECT_TYPE_PSWITCH;
}


void CPswitch::Render()
{

	CAnimations* animations = CAnimations::GetInstance();
	int ani = 0;
	if (state == SWITCH_STATE_OFF) {
		ani = ID_ITEM_ANI_PSWITCH_OFF;
	}
	else ani = ID_ITEM_ANI_PSWITCH_ON;

	animations->Get(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CPswitch::Update(DWORD dt, vector<LPGAMEOBJECT>* objects, vector<LPGAMEOBJECT>* items) {
	CGameObject::Update(dt, objects);
	vector<LPGAMEOBJECT> goldBrick;
	if (onChange) {// ktra nhan nut
		// gach bien mat -> state coin
		for (int i = 0; i < objects->size(); i++) {
			if (objects->at(i)->GetType() == OBJECT_TYPE_BRICK) {

				CBrick* brick = dynamic_cast<CBrick*>(objects->at(i));

				if (brick->GetBrickType() == BRICK_TYPE_GOLD && brick->GetState() != BRICK_STATE_INVISIBLE) {
					brick->SetState(BRICK_STATE_INVISIBLE);
				}

				CCoin* coin = new CCoin(brick->GetPosX(), brick->GetPosY());
				coin->setIsPswitch(true);
				objects->push_back(coin);
				//coins->push_back(coin);
			}
		}
		onChange = false;
	}
	if (changeTimer && changeTimer->IsTimeUp()) {
		//tien bien lai thanh gach
		for (int i = 0; i < objects->size(); i++) {
			if (dynamic_cast<CBrick*>(objects->at(i))) {

				CBrick* brick = dynamic_cast<CBrick*>(objects->at(i));

				if (brick->GetBrickType() == BRICK_TYPE_GOLD && brick->GetState() == BRICK_STATE_INVISIBLE) {

					brick->SetState(BRICK_STATE_ACTIVE);

				}
			}
			if (dynamic_cast<CCoin*>(objects->at(i))) {
				CCoin* coin = dynamic_cast<CCoin*>(objects->at(i));

				if (coin->getIsPswitch() == true) {
					coin->Delete();
				}
			}

		}
		//coins->clear();
		changeTimer->Stop();
	}
	
	CCollision::GetInstance()->Process(this, dt, objects);
}
void CPswitch::SetState(int state) {
	CGameObject::SetState(state);

	switch (state)
	{
	case SWITCH_STATE_OFF:
		changeTimer->Start();
		//gach bien thanh tien
		onChange = true;
		y = y + 10;
		break;
	case SWITCH_STATE_ON:
		break;
	default:
		break;
	}
}
void CPswitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == SWITCH_STATE_OFF)
		l = t = b = r = 0;
	else
	{
		l = x - BRICK_BBOX / 2;
		t = y - BRICK_BBOX / 2;
		r = l + BRICK_BBOX;
		b = t + BRICK_BBOX;
	}
}