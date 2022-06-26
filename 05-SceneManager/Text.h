#pragma once
#include "Sprites.h"
#include "string"
#include "Textures.h"
#include "Game.h"
#include "Utils.h"

#define CHAR_SIZE	8

//text
#define num_0	9920
#define num_1	9921
#define num_2	9922
#define num_3	9923
#define num_4	9924
#define num_5	9925
#define num_6	9926
#define num_7	9927
#define num_8	9928
#define num_9	9929


class Text
{
	unordered_map<char, int> spriteID{
		{ '0', num_0 },
		{ '1', num_1 },
		{ '2', num_2 },
		{ '3', num_3 },
		{ '4', num_4 },
		{ '5', num_5 },
		{ '6', num_6 },
		{ '7', num_7 },
		{ '8', num_8 },
		{ '9', num_9 } };


public:
	Text() = default;

	void RenderCharacter(char c, float x,float y);
	void Render(char* s, float x, float y);
};
typedef Text* LPTEXT;