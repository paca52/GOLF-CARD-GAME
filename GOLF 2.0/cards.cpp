#include "include/raylib.h"
#include "include/cards.h"
/* #include "include/functions.h" */
#include <stdlib.h>
#include <string.h>
#include <iostream>

Cards::Cards(Cards &tmp, int index){
	// loading a single card

	face_texture = tmp.face_texture;
	back_texture = tmp.back_texture;
	back_sel_texture = tmp.back_sel_texture;

	num_of_cards = 13;
	card_width = (float)face_texture.width / num_of_cards;
	card_height = (float)face_texture.height / 2;
	this->index = index;
	selected = false;
}

Cards::Cards(Texture2D texture, Texture2D back, int index, float x, float y){
	this->x = x;
	this->y = y;
	
	this->card_width = texture.width;
	this->card_height = texture.height;

	back_texture = back;
	face_texture = texture;

	this->index = index;
	num_of_cards = 13;
}

Cards::Cards(char *type){
	// full deck texture loading

	char path[100] = "cards/";
	strcat(path, type);
	strcat(path, ".png");

	face_texture = LoadTexture(path);
	face_texture.height *= 4;
	face_texture.width *= 4;

	num_of_cards = 13;
	card_width = (float)face_texture.width / num_of_cards;
	card_height = (float)face_texture.height / 2;

	index = -1;

}

bool Cards::is_mouse_on_card(){
	float mouse_x = GetMouseX(), mouse_y = GetMouseY();

	if(
		mouse_x >= x && mouse_x <= x + card_width
		&&
		mouse_y >= y && mouse_y <= y + card_height
	){
		return true;
	}
	return false;
}

void Cards::draw_card(){
	if(index == -1)
		return;

	if(face_up == true){
		if(this->is_mouse_on_card() || selected == true)
			DrawTextureRec(face_texture, Rectangle{this->index * card_width, card_height, card_width, card_height}, {x, y}, RAYWHITE);
		else
			DrawTextureRec(face_texture, Rectangle{this->index * card_width, 0, card_width, card_height}, {x, y}, RAYWHITE);
	}
	else{
		if(this->is_mouse_on_card())
			DrawTexture(back_sel_texture, x, y, RAYWHITE);
		else
			DrawTexture(back_texture, x, y, RAYWHITE);
	}
}

void Cards::draw_card_opp(){
	if(index == -1)
		return;

	if(face_up == true)
		DrawTextureRec(face_texture, Rectangle{this->index * card_width, 0, card_width, card_height}, {x, y}, RAYWHITE);
	else
		DrawTexture(back_texture, x, y, RAYWHITE);
}

/* void Cards::draw_card(int index, Cards& cards, float x, float y){ */

/* 	if(index > 12 || index < 0) */
/* 		return; */
/* 	else */
/* 		DrawTextureRec(cards.texture, Rectangle{index * card_width, 0, card_width, card_height}, {x, y}, RAYWHITE); */
/* } */

/* void Cards::draw_card(Cards& tmp, float x, float y){ */

/* 	if(this->index != -1){ */
/* 		if(tmp.selected == true || mouse_on_card(tmp) == true){ */
/* 			if(pile_x == tmp.x && pile_y == tmp.y && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true) */
/* 				tmp.selected = true; */
/* 			else if(discard_pile_x == tmp.x && discard_pile_y == tmp.y && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true) */
/* 				tmp.selected = true; */
/* 			DrawTextureRec(this->texture, Rectangle{this->index * card_width, card_height, card_width, card_height}, {x, y}, RAYWHITE); */
/* 		} */
/* 		else{ */
/* 			DrawTextureRec(this->texture, Rectangle{this->index * card_width, 0, card_width, card_height}, {x, y}, RAYWHITE); */
/* 			tmp.selected = false; */
/* 		} */
/* 	} */

/* } */

/* void Cards::draw_card_opp(float x, float y){ */

/* 	if(this->index != -1) */
/* 		DrawTextureRec(this->texture, Rectangle{this->index * card_width, 0, card_width, card_height}, {x, y}, RAYWHITE); */

/* } */

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
