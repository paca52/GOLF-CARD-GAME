#include "include/functions.h"
#include <cstring>
#include <iterator>
#include <utility>
#include <vector>
#include <iostream>
#include "include/hashtables.h"
#include <stack>

bool draw_gameover = false;

int score_me = 0;
int score_opp = 0;

Cards pile_card;
bool draw_pile_card = false;
bool pile_swap_mode = false;

enum TURN {opponent, mine};
TURN turn = mine;

enum DIS { 
	swap_a_card_on_the_field,
	discard_the_drawn_card,
	draw_card_from_the_pile,
	take_the_card_from_the_discard_pile,
	dis_has_been_done,
	none
};
DIS dis = none;
Cards *card_to_swap_with = NULL;

std::stack <Cards> discard_pile;
bool draw_discard_pile = false;
bool beginning = true;

table *hashtable[table_size];
bool has_been_init = false;

std::vector<Cards> me;
std::vector<Cards> opp;

int frame_count = 0, prev_frame_count = -1;
float timer = 0, timer_for_end = 0;
bool restart_timer = true;
// int dis = 0;

void razmeni(Cards &a, Cards &b){
	Cards tmp = a;
	a = b;
	b = tmp;
}

void sort_me(std::vector <Cards> &v){
	for(int i=0; i<5; i++)
		for(int j=i+1; j<6; j++)
			if(v[i].x >= v[j].x)
				razmeni(v[i], v[j]);
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

DIS make_dis(Cards **card_to_swap_with){

	if(pile_card.selected && pile_card.face_up){
		// checking to make 0s
		for(int i=0; i<6; i+=2){
			if(opp[i].face_up && opp[i+1].face_up && opp[i].index == opp[i+1].index)
				continue;
			
			if(opp[i].face_up && opp[i].index == pile_card.index){
				*card_to_swap_with = &opp[i+1];
				return swap_a_card_on_the_field;
			}

			if(opp[i+1].face_up && opp[i+1].index == pile_card.index){
				*card_to_swap_with = &opp[i];
				return swap_a_card_on_the_field;
			}
		}

		// opening the cards that are face down
		for(int i=0; i<6; i+=2){
			if(opp[i].face_up == false && opp[i+1].face_up == false){
				if(*card_to_swap_with == NULL){
					*card_to_swap_with = &opp[i + rand() % 2];
					return swap_a_card_on_the_field;
				}
			}
		}
		// value checking 
		for(int i=0; i<6; i+=2){
			if(opp[i].face_up && opp[i+1].face_up && opp[i].index == opp[i+1].index)
				continue;
			
			// if we found a way to make a 0, we do it
			if(opp[i].face_up && opp[i].index == pile_card.index){
				*card_to_swap_with = &opp[i];
				return swap_a_card_on_the_field;
			}
			
			// same here
			if(opp[i+1].face_up && opp[i+1].index == pile_card.index){
				*card_to_swap_with = &opp[i+1];
				return swap_a_card_on_the_field;
			}
			
			// if we found a card we want to swap with(that card and
			// the draw pile) we set it to be card_to_swap_with,
			// the second if is to check for the biggest one to get rid of
			// (example between 9 and a Jack he should pick a Jack,
			// regardless of their positions)
			if(opp[i].face_up && opp[i].get_value() > pile_card.get_value()){
				if(*card_to_swap_with == NULL)
					*card_to_swap_with = &opp[i];
				else if (( *card_to_swap_with )->get_value() < opp[i].get_value())
					*card_to_swap_with = &opp[i];
			}

			// -||-
			if(opp[i+1].face_up && opp[i+1].get_value() > pile_card.get_value()){
				if(*card_to_swap_with == NULL)
					*card_to_swap_with = &opp[i+1];
				else if (( *card_to_swap_with )->get_value() < opp[i+1].get_value())
					*card_to_swap_with = &opp[i+1];
			}
			
		}

		// if no card has been selected we throw the pile_card to the
		// discard pile
		if(*card_to_swap_with == NULL){
			*card_to_swap_with = &pile_card;
			return take_the_card_from_the_discard_pile;
		}

		return swap_a_card_on_the_field;
	}

	Cards discard_tmp = discard_pile.top();

	// checking with the discard pile
	for(int i=0; i<6; i+=2){
		if(opp[i].face_up && opp[i+1].face_up){
			// if we have two card that make 0 dont touch them
			if(opp[i].index == opp[i+1].index)
				continue;
			
			// if they dont make a zero, try to make it 
			if(opp[i].index == discard_tmp.index){
				*card_to_swap_with = &opp[i+1];
				return take_the_card_from_the_discard_pile;
			}

			// same here with the other card
			if(opp[i+1].index == discard_tmp.index){
				*card_to_swap_with = &opp[i];
				return take_the_card_from_the_discard_pile;
			}

		}
		// should we swap with the face down card
		else if(opp[i].face_up && opp[i].index == discard_tmp.index){
			*card_to_swap_with = &opp[i+1];
			return take_the_card_from_the_discard_pile;
		}
		// same here
		else if(opp[i+1].face_up && opp[i+1].index == discard_tmp.index){
			*card_to_swap_with = &opp[i];
			return take_the_card_from_the_discard_pile;
		}
	}

	// value checking with discard pile
	for(int i=0; i<6; i+=2){
		if(opp[i].face_up && opp[i+1].face_up && opp[i].index == opp[i+1].index)
			continue;

		if(opp[i].face_up && opp[i].get_value() > discard_tmp.get_value()){
			if(*card_to_swap_with == NULL)
				*card_to_swap_with = &opp[i];
			else if( (*card_to_swap_with)->get_value() < opp[i].get_value())
				*card_to_swap_with = &opp[i];
		}

		if(opp[i+1].face_up && opp[i+1].get_value() > discard_tmp.get_value()){
			if(*card_to_swap_with == NULL)
				*card_to_swap_with = &opp[i+1];
			else if( (*card_to_swap_with)->get_value() < opp[i+1].get_value())
				*card_to_swap_with = &opp[i+1];
		}

	}

	if(*card_to_swap_with != NULL)
		return take_the_card_from_the_discard_pile;

	return draw_card_from_the_pile;
}

void swap_two_cards(Cards &a, Cards &b){
	float a_x = a.x, a_y = a.y;	
	float b_x = b.x, b_y = b.y;

	Cards tmp = a;
	a = b;
	b = tmp;

	a.x = a_x;
	a.y = a_y;

	b.x = b_x;
	b.y = b_y;
	b.face_up = true;
	b.selected = false;
	a.face_up = true;
	a.selected = false;

}

void UPDATE_OPP(Cards cards[], Texture2D card_back[]){

	if(dis == dis_has_been_done){
		turn = mine;
		restart_timer = true;
		dis = none;
		return;
	}

	if(dis == none){
		dis = make_dis(&card_to_swap_with);
		return;
	}

	// make a dis to take a look at the top card of the discard_pile
	if(dis == draw_card_from_the_pile){
		pile_card.face_up = true;
		pile_card.selected = true;

		dis = none;
		return;
	}
	
	// swap the selected card with the pile_card and send the swaped one to the discard_pile
	if(dis == swap_a_card_on_the_field){
		if(card_to_swap_with == NULL){
			std::cout << "ERROR: card_to_swap_with == NULL when swaping the card on the field\n";
			return;
		}

		discard_pile.push(*card_to_swap_with);
		discard_pile.top().x = discard_pile_x;
		discard_pile.top().y = discard_pile_y;
		discard_pile.top().selected = false;
		discard_pile.top().face_up = true;

		float tmp_x = card_to_swap_with->x;
		float tmp_y = card_to_swap_with->y;

		*card_to_swap_with = pile_card;
		card_to_swap_with->x = tmp_x;
		card_to_swap_with->y = tmp_y;
		card_to_swap_with->selected = false;
		card_to_swap_with->face_up = true;

		pile_card = get_random_card(cards);
		pile_card.face_up = false;
		pile_card.x = pile_x;
		pile_card.y = pile_y;
		pile_card.selected = false;

		dis = dis_has_been_done;
		return;
	}

	if(dis == take_the_card_from_the_discard_pile){
		if(card_to_swap_with == NULL){
			std::cout << "ERROR: card_to_swap_with == NULL" << std::endl;
			return;
		}

		std::cout << "swaping...\n";

		// if the card has been drawn and the computer deems it useless
		// i set the card_to_swap_with to the pile_card and sent 
		// the take_the_card_from_the_discard_pile dis
		// and here it gets a new card if that's the case 
		if(card_to_swap_with->x == pile_x && card_to_swap_with->y == pile_y){
			swap_two_cards(*card_to_swap_with, discard_pile.top());
			pile_card = get_random_card(cards);
			pile_card.x = pile_x;
			pile_card.y = pile_y;
			pile_card.face_up = false;
		}
		// else it just wants to take the discard_pile card
		else
			swap_two_cards(*card_to_swap_with, discard_pile.top());

		dis = dis_has_been_done;
		card_to_swap_with = NULL;
		return;
	}
	if(dis == draw_card_from_the_pile){
		dis = dis_has_been_done;
		std::cout << "DIS: DRAW A CARD\n";
		return;
	}
}

int game_over(std::vector <Cards> v){
	int br = 0;
	
	for(Cards &tmp : v)
		if(tmp.face_up)
			br++;

	return br;
}

void UPDATE(Cards cards[], Texture2D card_back[]){
	// opponents turn
	if(turn == mine)
		std::cout << "YOUR TURN\n";
	else
		std::cout << "NOT YOUR TURN\n";

	if(game_over(me) == 6 || game_over(opp) == 6){
		draw_gameover = true;
		score_me = calc_score_me(me);
		score_opp = calc_score_me(opp);

		for(Cards &tmp : me)
			tmp.face_up = true;
		
		for(Cards &tmp : opp)
			tmp.face_up = true;
		
		timer_for_end += GetFrameTime();
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

		sort_me(me);
		sort_me(opp);
		
		std::cout << "OPP:\n";
		for(Cards &tmp : opp)
			std::cout << "\t(x, y) = (" << tmp.x << ", " << tmp.y << " )\n";

		std::cout << "\nME:\n";
		for(Cards &tmp : me)
			std::cout << "\t(x, y) = (" << tmp.x << ", " << tmp.y << " )\n";

		score_me = calc_score_me(me);

		std::cout << me.size() << " <- size\n";
		has_been_init = true;
	}
	
	if(beginning == false && turn == opponent && draw_gameover == false){
		if(restart_timer){
			frame_count = 0;
			timer = 0;
			restart_timer = false;
			dis = none;
			card_to_swap_with = NULL;
		}
		switch(dis){
			case 0:
				std::cout << "swap_a_card_on_the_field\n";
				break;
			case 1:
				std::cout << "discard_the_drawn_card\n";
				break;
			case 2:
				std::cout << "draw_card_from_the_pile\n";
				break;
			case 3:
				std::cout << "take_the_card_from_the_discard_pile\n";
				break;
			case 4:
				std::cout << "dis_has_been_done\n";
				break;
			case 5:
				std::cout << "none\n";
				break;
		}

		// if the turn has been done just skip the waiting
		if(dis == dis_has_been_done)
			UPDATE_OPP(cards, card_back);

		if(prev_frame_count != frame_count && frame_count % 2 == 0){
			prev_frame_count = frame_count;
			UPDATE_OPP(cards, card_back);
			score_opp = calc_score_me(opp);
		}

		timer += GetFrameTime();

		if(timer >= 2.0f){
			timer = 0;
			frame_count++;
		}
		
		frame_count = frame_count % 100;
		std::cout << "timer: " << timer << std::endl;
		std::cout << "frame: " << frame_count << std::endl;
		std::cout << std::endl;
		return;
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
		if(draw_pile_card == false){

			pile_card = get_random_card(cards);

			pile_card.x = pile_x;
			pile_card.y = pile_y;

			draw_pile_card = true;
		}

		if(discard_pile.empty()){

			Cards new_card = get_random_card(cards);
			new_card.x = discard_pile_x;
			new_card.y = discard_pile_y;
			new_card.face_up = true;
			new_card.selected = false;

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

			if(pile_card.selected == false && pile_card.face_up == false && discard_pile.empty() == false && discard_pile.top().is_mouse_on_card() == true){
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
	
	if(draw_gameover && timer_for_end >= 10.0f){

	BeginDrawing();

		ClearBackground(BACK);
		DrawText("GAME OVER", ((float)screenWidth / 2 - 30), (float)screenHeight / 4, 10, RED);
		if(score_me < score_opp)
			DrawText("YOU WON", (float)screenWidth / 2 - 15, (float)screenHeight / 2, 15, GREEN);
		else if(score_opp < score_me)
			DrawText("YOU LOST", (float)screenWidth / 2 - 15, (float)screenHeight / 2, 15, RED);
		else
			DrawText("DRAW", (float)screenWidth / 2 - 15, (float)screenHeight / 2, 15, GRAY);


	EndDrawing();
	return;	
	}

	BeginDrawing();

		ClearBackground(BACK);

		// playing field
		DrawRectangle(screenWidth / 4, 0, screenWidth / 2, screenHeight / 2, { 230, 41, 55, (unsigned char)(50 + 50 * (turn == opponent)) });
		DrawRectangle(screenWidth / 4, screenHeight / 2, screenWidth / 2, screenHeight / 2, { 0, 121, 241, (unsigned char)( 50 +(turn == mine)*50 )});

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

	if(IsKeyPressed(KEY_ENTER)){
		menu = false;
		return;
	}

	BeginDrawing();

		ClearBackground(BACK);

		DrawRectangle(rec_x, rec_y, rec_width, rec_height, BUTTON_COLOR);
		DrawText("PLAY", (float)screenWidth / 2 - (float)rec_width / 2 + 10, (float)screenHeight / 2 - (float)rec_height / 2, 30, RAYWHITE);

	EndDrawing();
}

