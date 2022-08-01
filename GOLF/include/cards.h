#ifndef CARDS_H
#define CARDS_H

#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;

const float pile_x = 650;
const float pile_y = (float)screenHeight / 2 - 50; //50 = (float)card_back[0].height / 2;

const float discard_pile_x = (float)screenWidth / 4 - 100;
const float discard_pile_y =  pile_y;

typedef class Cards{
public:
	Cards() = default;
	Cards(Texture2D texure, int index);
	Cards(char *type);
	Cards(float x, float y, Texture2D texture);

	void draw_card(int index, Cards& cards, float x, float y);
	void draw_card(Cards& tmp, float x, float y);
	void draw_card_opp(float x, float y);
	int get_value();

	Texture2D texture;
	int num_of_cards;
	float card_width;
	float card_height;
	int index = -1;
	char type[100];
	float x, y;
	bool selected = false;
	bool face_up = false;
}Cards;

#endif // CARDS_H
