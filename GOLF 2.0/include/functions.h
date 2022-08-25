#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "raylib.h"
#include "cards.h"
#include <vector>
#include "hashtables.h"

#define BACK Color {232, 219, 219, 255}
#define BUTTON_COLOR Color {134, 168, 231, 255}
#define UNKNOWN Color {95, 251, 241, 255}

void DRAW(Cards cards[], Texture2D card_back[]);
void UPDATE(Cards cards[], Texture2D card_back[]);
void UPDATE_OPP(Cards cards[], Texture2D card_back[]);
void DRAW_MENU(bool& menu);
Cards get_random_card(Cards cards[]);
std::vector<Cards> init_vector(std::vector <Cards> vec, Texture2D card_back, Cards cards[]);
bool mouse_on_card(Cards tmp);
void razmeni(Cards &a, Cards &b);
void sort_me(std::vector <Cards> &v);

void swap_cards_discard(Cards &tmp, Cards &temp);
void swap_cards_me(Cards& tmp);
void swap_two_cards(Cards &a, Cards &b);
void calc_score();

#endif // FUNCTIONS_H
