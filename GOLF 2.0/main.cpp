#include "include/raylib.h"
#include "include/cards.h"
#include "include/functions.h"

int main(void)
{

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	Cards spades((char*)"spades");
	Cards diamonds((char*)"diamonds");
	Cards hearts((char*)"hearts");
	Cards clubs((char*)"clubs");

	Cards cards[] = { spades, diamonds, clubs, hearts };

	Texture2D card_back[] = { LoadTexture("cards/card_back.png"), LoadTexture("cards/card_back_selected.png") };	
	for(int i=0; i<2; i++){
		card_back[i].height *= 4;
		card_back[i].width *= 4;
	}

	for(Cards &tmp : cards){
		tmp.back_texture = card_back[0];
		tmp.back_sel_texture = card_back[1];
	}

	bool menu = true;

    SetTargetFPS(10);

    while (!WindowShouldClose())
    {

		if( menu == true )
			DRAW_MENU(menu);
		else{
			UPDATE(cards, card_back);
			DRAW(cards, card_back);
		}
    }

    CloseWindow();

    return 0;
}

