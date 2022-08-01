#include "include/functions.h"
#include <cstring>
#include <iterator>
#include <utility>
#include <vector>
#include <iostream>
#include "include/hashtables.h"
#include <stack>

int score_me = 0;

Cards pile_card;
bool draw_pile_card = false;
bool pile_card_face_up = false;

std::stack <Cards> discard_pile;
bool draw_discard_pile = false;
bool begining = true;

table *hashtable[table_size];
bool has_been_init = false;

std::vector<Cards> me;
std::vector<Cards> opp;

void razmeni(Cards &a, Cards &b){
	Cards tmp = a;
	a = b;
	b = tmp;
}

void print_me(){
	std::cout << "start: \n";
	for(int i=0; i<6; i++)
		std::cout << "\t" << me[i].get_value() << "\n";
		
}

void sort_me(){
	for(int i=0; i<5; i++)
		for(int j=i+1; j<6; j++)
			if(me[i].x >= me[j].x)
				razmeni(me[i], me[j]);
}

void calc_score_me(){
	score_me = 0;
	for(int i=0; i<6; i+=2){
		if(me[i].face_up == true && me[i+1].face_up == true){
			if(me[i].index == me[i+1].index)
				score_me += 0;
			else
				score_me += (me[i].get_value() + me[i+1].get_value());
		}
		else if(me[i].face_up == true){
			score_me += me[i].get_value();	
		}
		else if(me[i+1].face_up == true){
			score_me += me[i+1].get_value();
		}
	}
}

void swap_cards_me(Cards &tmp){
	tmp.texture = pile_card.texture;
	tmp.index = pile_card.index;
	tmp.face_up = true;
	std::cout << "CARD HAS BEEN SWAPED!" << std::endl;
}

void swap_cards_discard(Cards &tmp, Cards &temp){
	int index_tmp = tmp.index;
	Texture2D texture_tmp = tmp.texture;

	tmp.texture = temp.texture;
	tmp.index = temp.index;

	temp.index = index_tmp;
	temp.texture = texture_tmp;

	tmp.face_up = true;
	temp.face_up = true;

	std::cout << "CARD HAS BEEN SWAPED(DISCARD)!" << std::endl;
}

bool mouse_on_card(Cards tmp){
	float mouse_x = GetMouseX();
	float mouse_y = GetMouseY();

	if(
		mouse_x >= tmp.x && mouse_x <= tmp.x + tmp.card_width
		&&
		mouse_y >= tmp.y && mouse_y <= tmp.y + tmp.card_height
	){
		return true;
	}

	return false;		
}

Cards get_random_card(Cards cards[]){
	// 0 - spades
	// 1 - diamons
	// 2 - clubs
	// 3 - hearts
	int tmp, index;
	if(table_full(hashtable) == true)
		return Cards();
	do{
		tmp = rand() % 13;
		index = tmp;

		tmp = rand() % 4;

	}while(table_lookup(hashtable, tmp, index) == true);

	table_insert(hashtable, tmp, index);
	
	Cards new_card(cards[tmp].texture, index);
	return new_card;
}

void UPDATE(Cards cards[], Texture2D card_back[]){
	if(has_been_init == false){
		table_init(hashtable);
		has_been_init = true;
	}

	// discard pile swaping
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) == true && discard_pile.size() > 0 && discard_pile.top().selected == true && pile_card.selected == false){
		std::cout << "STARTING TO SWAP CARDS(discard pile -> playing field)" << std::endl;
		Cards &tmp = discard_pile.top();
		
		float mouse_x = GetMouseX();
		float mouse_y = GetMouseY();

			
		for(Cards& tmp : me){
			if(
				mouse_x >= tmp.x && mouse_x <= tmp.x + tmp.card_width
				&&
				mouse_y >= tmp.y && mouse_y <= tmp.y + tmp.card_height
			){

				Cards &temp = discard_pile.top();
				swap_cards_discard(tmp, temp);

				std::cout << "DISCARD PILE CARD HAS BEEN SWAPED!" << std::endl;

				discard_pile.top().selected = false;
				calc_score_me();
				break;
			}
		}


	}
	
	/* if(pile_card_face_up == true && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true && discard_pile.size() > 0 && mouse_on_card(discard_pile.top())){ */
	/* 	discard_pile.push(pile_card); */		
	/* 	discard_pile.top().face_up = true; */
	/* 	pile_card_face_up = false; */
	/* 	pile_card = get_random_card(cards); */
	/* } */


	// pile card swaping
	if(pile_card.selected == true && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true){
		float mouse_x = GetMouseX();
		float mouse_y = GetMouseY();

		for(Cards& tmp : me){
			if(
				mouse_x >= tmp.x && mouse_x <= tmp.x + tmp.card_width
				&&
				mouse_y >= tmp.y && mouse_y <= tmp.y + tmp.card_height
			){
				draw_discard_pile = true;

				discard_pile.push(tmp);
				discard_pile.top().face_up = true;

				swap_cards_me(tmp);
				pile_card.selected = false;
				pile_card.texture = card_back[0];
				pile_card_face_up = false;
				calc_score_me();
				break;
			}
		}

	}

	// selection of cards in the begining
	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true && begining == true){
		float mouse_x = GetMouseX();
		float mouse_y = GetMouseY();

		for(int i=0; i<3; i++){
			if(
				mouse_x >= 100 * (i + 6) - 330 && mouse_x <= 100 * (i + 6) - 330  + card_back[0].width
					&&
				mouse_y >= 340 && mouse_y <= 340 + card_back[0].height
			){
				Cards tmp = get_random_card(cards);

				std::cout << "Card selected!" << std::endl;
				
				tmp.x = 100 * (i + 6) - 330;
				tmp.y = 340;
				tmp.selected = true;
				tmp.face_up = true;

				me.push_back(tmp);
			}
			else if(
				mouse_x >= 100 * (i + 6) - 330 && mouse_x <= 100 * (i + 6) - 330  + card_back[0].width
					&&
				mouse_y >= 235 && mouse_y <= 235 + card_back[0].height
			){

				Cards tmp = get_random_card(cards);
				
				std::cout << "Card selected!" << std::endl;

				tmp.x = 100 * (i + 6) - 330;
				tmp.y = 235;
				tmp.selected = true;
				tmp.face_up = true;

				me.push_back(tmp);
			}
		}

		if(me.size() == 2){
			// NOTE: me.end() returns the n-th element( one after the last one ), thus if you want to acces the last element
			//		 you need to decrement it by 1
			auto it = me.end() - 1;

			if(me[0].x == it->x && me[0].y == it->y){
				std::cout << "ERASED!\n";
				me.erase(it);
			}
		}
	}


	if(pile_card_face_up == false && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true && begining == false)
		if(mouse_on_card(Cards{pile_x, pile_y, card_back[0]}) == true){
			pile_card = get_random_card(cards);
			pile_card.x = pile_x;
			pile_card.y = pile_y;
			draw_pile_card = true;
			pile_card_face_up = true;
			std::cout << "DRAW PILE CARD ! " << std::endl;
		}


	// closing the begining
	if(me.size() >= 2 && begining == true){
		Cards tmp = get_random_card(cards);
		tmp.texture = card_back[0];
		
		// putting in the face-down cards in
		for(int i=0; i<3; i++){
			tmp.x = (i + 6) * 100 - 330;
			tmp.y = 235;
			me.push_back(tmp);
			tmp = get_random_card(cards);
		}

		for(int i=0; i<3; i++){
			tmp.x = (i + 6) * 100 - 330;
			tmp.y = 340;
			me.push_back(tmp);
			tmp = get_random_card(cards);
		}
		
		// removing the unnecessary ones
		for(int i=0; i<2; i++){
			for(auto it = begin(me) + 2; it != end(me); it++)
				if(me[i].x == it->x && me[i].y == it->y){
					me.erase(it);
					break;
				}
		}

		// filling the opponent's cards
		tmp = get_random_card(cards);


		int pos_of_card1 = rand() % 6;
		int pos_of_card2 = rand() % 6;
		while(pos_of_card2 == pos_of_card1)
			pos_of_card2 = rand() % 6;

		for(int i=0; i<3; i++){

			tmp.x = (i + 6) * 100 - 330;
			tmp.y = 10;

			if(!( pos_of_card1 == i || pos_of_card2 == i ))
				tmp.texture = card_back[0];
			opp.push_back(tmp);
			tmp = get_random_card(cards);
		}

		for(int i=0; i<3; i++){
			tmp.x = (i + 6) * 100 - 330;
			tmp.y = 115;

			if(!( pos_of_card1 == (3+i) || pos_of_card2 == (3+i) ))
				tmp.texture = card_back[0];
			opp.push_back(tmp);
			tmp = get_random_card(cards);
		}

		// SCORE
		sort_me();
		calc_score_me();

		print_me();
		
		begining = false;
	}
}

void DRAW(Cards cards[], Texture2D card_back[]){
	// Draw
	//----------------------------------------------------------------------------------

	BeginDrawing();

		ClearBackground(BACK);
		DrawRectangle(screenWidth / 4, 0, screenWidth / 2, screenHeight / 2, { 230, 41, 55, 50 });
		DrawRectangle(screenWidth / 4, screenHeight / 2, screenWidth / 2, screenHeight / 2, { 0, 121, 241, 50 });
		DrawCircle(0, 720, 10, GREEN);

		if(begining == true){
			// opp
			for(int i=0; i<3; i++){
				DrawTexture(card_back[0], 100 * (i + 6) - 330, 10, RAYWHITE);
				DrawTexture(card_back[0], 100 * (i + 6) - 330, 115, RAYWHITE);
			}

			// mine 
			for(int i=0; i<3; i++){
				if(mouse_on_card(Cards { (float)( 100*(i+6) - 330 ), 340, card_back[0]}) == true)
					DrawTexture(card_back[1], 100 * (i + 6) - 330, 340, RAYWHITE);
				else
					DrawTexture(card_back[0], 100 * (i + 6) - 330, 340, RAYWHITE);

				if(mouse_on_card(Cards { (float)( 100*(i+6) - 330), 235, card_back[0]}) == true)
					DrawTexture(card_back[1], 100 * (i + 6) - 330, 235, RAYWHITE);
				else
					DrawTexture(card_back[0], 100 * (i + 6) - 330, 235, RAYWHITE);
			}
			
		}
		else{
			DrawText(TextFormat("SCORE: %d", score_me), (float)screenWidth / 8 - 50, (float)screenHeight / 4 * 3, 20, PINK);

			for(Cards tmp : me){
				tmp.selected = false;
				if(tmp.face_up == false){
					if(mouse_on_card(tmp) == true)
						DrawTexture(card_back[1], tmp.x, tmp.y, RAYWHITE);
					else
						DrawTexture(card_back[0], tmp.x, tmp.y, RAYWHITE);
				}
				else
					tmp.draw_card(tmp, tmp.x, tmp.y);
			}

			for(Cards tmp : opp)
				tmp.draw_card_opp(tmp.x, tmp.y);

			// draw pile
			if(draw_pile_card == true)
				pile_card.draw_card(pile_card, pile_x, pile_y);
			else{
				if(mouse_on_card(Cards{pile_x, pile_y, card_back[0]}) == true)
					DrawTexture(card_back[1], pile_x, pile_y, RAYWHITE);
				else
					DrawTexture(card_back[0], pile_x, pile_y, RAYWHITE);
			}

			//discard pile

			if(draw_discard_pile == true){
				if(discard_pile.size() > 0){
					Cards &tmp = discard_pile.top();
					tmp.x = discard_pile_x;
					tmp.y = discard_pile_y;
					tmp.card_width = card_back[0].width;
					tmp.card_height = card_back[0].height;
					tmp.draw_card(tmp, discard_pile_x, discard_pile_y);
					/* std::cout << tmp.selected << " <- tmp.selected" << std::endl; */
				}
				else
					draw_discard_pile = false;
			}

		}

	EndDrawing();
}

void DRAW_MENU(bool& menu){
	int rec_width = 100;
	int rec_height = 30;
	float rec_x = (float)screenWidth / 2 - (float)rec_width / 2;
	float rec_y = (float)screenHeight / 2 - (float)rec_height / 2;


	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true){
		float mouse_x = GetMouseX();
		float mouse_y = GetMouseY();

		if(mouse_x >= rec_x && mouse_x <= rec_x + rec_width
			&&
		   mouse_y >= rec_y && mouse_y <= rec_y + rec_height == true){
			menu = false;
			return;
		}
	}

	BeginDrawing();

		ClearBackground(BACK);

		DrawRectangle(rec_x, rec_y, rec_width, rec_height, BUTTON_COLOR);
		DrawText("PLAY", (float)screenWidth / 2 - (float)rec_width / 2 + 10, (float)screenHeight / 2 - (float)rec_height / 2, 30, RAYWHITE);

	EndDrawing();
}

std::vector<Cards> init_vector(std::vector <Cards> vec, Texture2D card_back, Cards cards[]){
	Cards tmp = get_random_card(cards);
	tmp.texture = card_back;

	for(int i=0; i<3; i++){
		tmp.x = (i + 6) * 100 - 330;
		tmp.y = 235;
		vec.push_back(tmp);
		tmp = get_random_card(cards);
		tmp.texture = card_back;
	}

	for(int i=0; i<3; i++){
		tmp.x = (i + 6) * 100 - 330;
		tmp.y = 340;
		vec.push_back(tmp);
		tmp = get_random_card(cards);
		tmp.texture = card_back;
	}

	return vec;
}

