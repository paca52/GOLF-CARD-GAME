#include "include/raylib.h"
#include "include/cards.h"
#include "include/functions.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

Cards::Cards(Texture2D texure, int index){
	this->texture = texure;

	num_of_cards = 13;
	card_width = (float)texture.width / num_of_cards;
	card_height = (float)texture.height / 2;
	this->index = index;
	strcpy_s(type, 100, "singe card");
}

Cards::Cards(float x, float y, Texture2D texture){
	this->x = x;
	this->y = y;
	
	this->card_width = texture.width;
	this->card_height = texture.height;
}

Cards::Cards(char *type){

	char path[100] = "cards/";
	strcat_s(path, type);
	strcat_s(path, ".png");

	texture = LoadTexture(path);
	texture.height *= 4;
	texture.width *= 4;

	num_of_cards = 13;
	card_width = (float)texture.width / num_of_cards;
	card_height = (float)texture.height / 2;

	strcpy_s(this->type, 100, "full deck");
	index = -1;

}

void Cards::draw_card(int index, Cards& cards, float x, float y){

	if(index > 12 || index < 0)
		return;
	else
		DrawTextureRec(cards.texture, Rectangle{index * card_width, 0, card_width, card_height}, {x, y}, RAYWHITE);
}

void Cards::draw_card(Cards& tmp, float x, float y){

	if(this->index != -1){
		if(tmp.selected == true || mouse_on_card(tmp) == true){
			if(pile_x == tmp.x && pile_y == tmp.y && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true)
				tmp.selected = true;
			else if(discard_pile_x == tmp.x && discard_pile_y == tmp.y && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true)
				tmp.selected = true;
			DrawTextureRec(this->texture, Rectangle{this->index * card_width, card_height, card_width, card_height}, {x, y}, RAYWHITE);
		}
		else{
			DrawTextureRec(this->texture, Rectangle{this->index * card_width, 0, card_width, card_height}, {x, y}, RAYWHITE);
			tmp.selected = false;
		}
	}

}

void Cards::draw_card_opp(float x, float y){

	if(this->index != -1)
		DrawTextureRec(this->texture, Rectangle{this->index * card_width, 0, card_width, card_height}, {x, y}, RAYWHITE);

}

int Cards::get_value(){
	if(index != -1){
		if(index == 12)
			return 0;
		if(index >= 10)
			return 10;
		if(index == 1)
			return -2;
		return index + 1;
	}
	return -1;
}
