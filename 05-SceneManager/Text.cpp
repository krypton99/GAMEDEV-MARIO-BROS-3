#include "Text.h"
#include "Animations.h"

void Text::RenderCharacter(char c, float x, float y)
{
	CAnimations* ani = CAnimations::GetInstance();
	ani->Get(spriteID.at(c))->Render(x, y);
}

void Text::Render(char* s, float x, float y)
{

	for (int i = 0; i< strlen(s); i++) {
		RenderCharacter(s[i],  x + i * CHAR_SIZE, y );
	}
}