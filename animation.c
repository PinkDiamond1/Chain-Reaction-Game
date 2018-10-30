#include <stdio.h>
#include <SDL2/SDL.h>
#include "chain.h"
#include <time.h>

extern int rows, columns, center;
extern SDL_Rect Rect1, Rect2;
extern SDL_Texture *savegame, *create_new_game;

void AD_AnimateScreen(SDL_Renderer **ren, board b, player *current, bucket *buc) {
	int pos, i, j, k;
	buffer B;
	int store = storage(buc);
	for(pos = 0; pos <= CELL_SIDE; pos += 3) {
		SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
		SDL_RenderClear(*ren);
		for(k = 0; k < store; k++) {
			B = check(buc, k);
			i = B.x;
			j = B.y;
			AD_DrawCircle(ren, CELL_SIDE * (j + 0.5) + pos, CELL_SIDE * (i + 0.5), CELL_SIDE * (28.0 / 80.0), current->r, current->g, current->b, current->a);
			AD_DrawCircle(ren, CELL_SIDE * (j + 0.5), CELL_SIDE * (i + 0.5) + pos, CELL_SIDE * (28.0 / 80.0), current->r, current->g, current->b, current->a);
			AD_DrawCircle(ren, CELL_SIDE * (j + 0.5) - pos, CELL_SIDE * (i + 0.5), CELL_SIDE * (28.0 / 80.0), current->r, current->g, current->b, current->a);
			AD_DrawCircle(ren, CELL_SIDE * (j + 0.5), CELL_SIDE * (i + 0.5) - pos, CELL_SIDE * (28.0 / 80.0), current->r, current->g, current->b, current->a);
		}	
		AD_CreateBoard(ren, 255, 255, 255, 0);
		UpdateBoardStatus(ren, b);
		SDL_RenderCopy(*ren, savegame, NULL, &Rect1);
		SDL_RenderCopy(*ren, create_new_game, NULL, &Rect2);
		SDL_RenderDrawRect(*ren, &Rect1);
		SDL_RenderDrawRect(*ren, &Rect2);
		AD_DrawCircle(ren, center, Rect1.y + CELL_SIDE / 2, CELL_SIDE / 2 - CELL_SIDE /10, current->r, current->g, current->b, current->a);
		SDL_RenderPresent(*ren);		
		SDL_Delay(1000 / 60);	
	}
	SDL_RenderClear(*ren);
}
