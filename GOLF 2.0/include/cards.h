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
	Cards(Cards &tmp, int index);
	Cards(char *type);
	Cards(Texture2D texure, Texture2D back, int index, float x, float y);

	void draw_card(int index, Cards& cards, float x, float y);
	void draw_card(Cards& tmp, float x, float y);
	void draw_card_opp(float x, float y);
	int get_value();
	void draw_card();
	void draw_card_opp();
	bool is_mouse_on_card();

	Texture2D face_texture, back_texture, back_sel_texture;
	int num_of_cards, index = -1;
	float card_width, card_height;
	float x, y;
	bool selected = false;
	bool face_up = false;
}Cards;

#endif // CARDS_H
