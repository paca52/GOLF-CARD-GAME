#include "include/functions.h"
#include <cstring>
#include <iterator>
#include <utility>
#include <vector>
#include <iostream>
#include "include/hashtables.h"
#include <stack>

int score_me = 0;
int score_opp = 0;

Cards pile_card;
bool draw_pile_card = false;
bool pile_swap_mode = false;

enum TURN {opponent, mine};

TURN turn = mine;

std::stack <Cards> discard_pile;
bool draw_discard_pile = false;
bool beginning = true;

table *hashtable[table_size];
bool has_been_init = false;

std::vector<Cards> me;
std::vector<Cards> opp;

int frame_count = 0;
float timer = 0;
bool restart_timer = true;
int dis = 0;

void razmeni(Cards &a, Cards &b){
	Cards tmp = a;
	a = b;
	b = tmp;
}

void sort_me(){
	for(int i=0; i<5; i++)
		for(int j=i+1; j<6; j++)
			if(me[i].x >= me[j].x)
				razmeni(me[i], me[j]);
}

int calc_score_me(std::vector <Cards> v){
	int score_v = 0;
	for(int i=0; i<6; i+=2){
		if(v[i].face_up == true && v[i+1].face_up == true){
			if(v[i].index == v[i+1].index)
				score_v += 0;
			else
				score_v += (v[i].get_value() + v[i+1].get_value());
		}
		else if(v[i].face_up == true){
			score_v += v[i].get_value();	
		}
		else if(v[i+1].face_up == true){
			score_v += v[i+1].get_value();
		}
	}
	return score_v;
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
	
	Cards new_card(cards[tmp], index);
	return new_card;
}

void swap_with_pile(Cards &card_to_swap, Cards cards[]){
	if(pile_card.selected && pile_card.face_up){
		float tmp_x, tmp_y;
		Cards &tmp = card_to_swap;
		tmp_x = tmp.x;
		tmp_y = tmp.y;
		tmp.face_up = true;

		discard_pile.push(tmp);
		discard_pile.top().x = discard_pile_x;
		discard_pile.top().y = discard_pile_y;
		discard_pile.top().selected = false;

		tmp = pile_card;
		tmp.x = tmp_x;
		tmp.y = tmp_y;

		pile_card = get_random_card(cards);
		pile_card.x = pile_x;
		pile_card.y = pile_y;
		pile_card.face_up = false;

		score_opp = calc_score_me(opp);
	}
	else
		std::cout << "pile_card needs to be selected first in order to be swaped with\n";

}

int make_dis(){
	Cards tmp = discard_pile.top();

	for(int i=0; i<6; i++)
		if(tmp.index == opp[i].index){}
			//dis = 
}

void UPDATE_OPP(Cards cards[], Texture2D card_back[]){

	if(frame_count != 0 && frame_count == 1)
	{
		std::cout << "thing one done!\n";
		pile_card.face_up = true;
		pile_card.selected = true;
	}
	if(frame_count != 0 && frame_count == 2){
		swap_with_pile(opp[3], cards);	

		turn = mine;
		restart_timer = true;
	}

}

void UPDATE(Cards cards[], Texture2D card_back[]){
	// opponents turn
	if(turn == opponent){
		if(restart_timer){
			frame_count = 0;
			timer = 0;
			restart_timer = false;
			dis = make_dis();
		}
		UPDATE_OPP(cards, card_back);

		timer += GetFrameTime();

		if(timer >= 3.0f){
			timer = 0;
			frame_count++;
		}
		
		frame_count = frame_count % 100;
		std::cout << "timer: " << timer << std::endl;
		std::cout << "frame: " << frame_count << std::endl;
		return;
	}

	if(has_been_init == false){
		table_init(hashtable);

		for(int i=0; i<3; i++){
			std::cout << 100*(i+6) - 330 << " <- x\n";

			Cards tmp = get_random_card(cards);
			tmp.x = 100*(i+6) - 330;
			tmp.y = 340;
			me.push_back(tmp);

			tmp = get_random_card(cards);
			tmp.x = 100*(i+6) - 330;
			tmp.y = 235;
			me.push_back(tmp);

			tmp = get_random_card(cards);
			tmp.x = 100*(i+6) - 330;
			tmp.y = 10;
			opp.push_back(tmp);

			tmp = get_random_card(cards);
			tmp.x = 100*(i+6) - 330;
			tmp.y = 115;
			opp.push_back(tmp);
		}

		sort_me();
		score_me = calc_score_me(me);

		std::cout << me.size() << " <- size\n";
		has_been_init = true;
	}
	
	if(beginning == true){

		//flip a cards in the beginning if it's clicked
		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) == true){
			for(Cards &tmp : me){
				if(tmp.is_mouse_on_card() == true){
					tmp.face_up = true;
					score_me = calc_score_me(me);
				}
			}
		}

		// generate opponents 1st two cards
		int num_of_face_ups = 0;
		for(Cards &tmp : me){
			if(tmp.face_up == true)
				num_of_face_ups++;
		}
		if(num_of_face_ups >= 2){

			std::cout << "gen 2 opp cards\n";

			int index1 = rand() % 6;
			int index2;
			do{
				index2 = rand() % 6;

			}while(index2 == index1);

			opp[index1].face_up = true;
			opp[index2].face_up = true;

			score_opp = calc_score_me(opp);

			beginning = false;
		}

	}
	else{
		// checking if the game should end
		// . . . 

		if(draw_pile_card == false){

			pile_card = get_random_card(cards);

			pile_card.x = pile_x;
			pile_card.y = pile_y;

			draw_pile_card = true;
		}

		if(discard_pile.empty()){
			int index_tmp = rand() % 13;
			int tmp = rand() % 4;

			Cards new_card;
			new_card.face_up = false;
			new_card.face_texture = cards[tmp].face_texture;
			new_card.index = index_tmp;
			new_card.back_sel_texture = cards[tmp].back_texture;
			new_card.back_texture = cards[tmp].back_texture;
			new_card.x = discard_pile_x;
			new_card.y = discard_pile_y;
			new_card.card_height = pile_card.card_height;
			new_card.card_width = pile_card.card_width;

			draw_discard_pile = true;

			discard_pile.push(new_card);
		}

		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) == true){
			std::cout << "mouse has been pressed" << std::endl;

			// swaping with discard_pile
			if(discard_pile.top().selected == true ){
				for(Cards &tmp : me)
					if(tmp.is_mouse_on_card()){
						Cards temp = tmp;
						float tmp_x = tmp.x, tmp_y = tmp.y;

						tmp = discard_pile.top();
						tmp.x = tmp_x;
						tmp.y = tmp_y;
						tmp.selected = false;

						discard_pile.pop();

						temp.x = discard_pile_x;
						temp.y = discard_pile_y;
						temp.selected = false;
						temp.face_up = true;
						discard_pile.push(temp);

						score_me = calc_score_me(me);

						turn = opponent;
						break;
					}
			}

			if(pile_card.selected == false && pile_card.face_up == false && discard_pile.size() > 1 && discard_pile.top().is_mouse_on_card() == true){
				discard_pile.top().selected = true;
			}

			// if pile_card has been clicked turn the card face up
			if(pile_card.is_mouse_on_card()){
				pile_card.face_up = true;
				pile_card.selected = true;
			}

			// swaping with draw pile and sending it to discard_pile
			if(pile_card.face_up){
				for(Cards &tmp : me)
					if(tmp.is_mouse_on_card()){
						Cards temp = tmp;
						temp.x = discard_pile_x;
						temp.y = discard_pile_y;
						temp.face_up = true;
						discard_pile.push(temp);

						tmp.face_texture = pile_card.face_texture;
						tmp.index = pile_card.index;
						tmp.face_up = true;
						
						pile_card.selected = false;
						pile_card = get_random_card(cards);
						pile_card.x = pile_x;
						pile_card.y = pile_y;

						score_me = calc_score_me(me);

						turn = opponent;
						break;
					}

				// sending the current top pile card to the discard_pile if it has been clicked on
				if(discard_pile.top().is_mouse_on_card()){
					pile_card.selected = false;

					Cards temp = pile_card;
					temp.x = discard_pile_x;
					temp.y = discard_pile_y;
					discard_pile.push(temp);

					pile_card = get_random_card(cards);
					pile_card.x = pile_x;
					pile_card.y = pile_y;

					score_me = calc_score_me(me);

					turn = opponent;
				}
			}

		}

	}

}

void DRAW(Cards cards[], Texture2D card_back[]){
	// Draw
	//----------------------------------------------------------------------------------

	BeginDrawing();

		ClearBackground(BACK);

		// playing field
		DrawRectangle(screenWidth / 4, 0, screenWidth / 2, screenHeight / 2, { 230, 41, 55, 50 });
		DrawRectangle(screenWidth / 4, screenHeight / 2, screenWidth / 2, screenHeight / 2, { 0, 121, 241, 50 });

		for(Cards &tmp : me)
			tmp.draw_card();

		for(Cards &tmp : opp)
			tmp.draw_card_opp();
		
		DrawText(TextFormat("SCORE: %d", score_me), (float)screenWidth / 8 - 50, (float)screenHeight / 4 * 3, 20, PINK);
		DrawText(TextFormat("SCORE: %d", score_opp), (float)screenWidth / 8 - 50, (float)screenHeight / 4, 20, PINK);

		if(draw_discard_pile)
			discard_pile.top().draw_card();

		if(draw_pile_card)
			pile_card.draw_card();

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

