#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "chain.h"

extern int rows, columns;
extern SDL_Color *ColorRow;

board Initiate_My_Board(void) {
	int i, j;
	board b = (struct cell **)malloc(sizeof(struct cell *) * rows);
	if(b == NULL) {
		printf("Not Enough Ram!!!\n");
		exit(4);
	}	
	for(i = 0; i < rows; i++) {
		b[i] = (struct cell *)malloc(sizeof(struct cell) * columns);
		if(b[i] == NULL) {
			printf("Not Enough Ram!!!\n");
			exit(4);
		}	
		for(j = 0; j < columns; j++) {
			b[i][j].balls = 0;
			b[i][j].p = NULL;
			if(i == 0 || i == (rows - 1)) {
				if(j == 0 || j == (columns - 1))
					b[i][j].capacity = 1;
				else
					b[i][j].capacity = 2;
			}
			else if(j == 0 || j == (columns - 1)) 
				b[i][j].capacity = 2;
			else 
				b[i][j].capacity = 3;
		}			
	}
	return b;
}	

player *Create_Player_Row(int players_number, int computer_players_number) {
	int i;
	player *pl = (player *)malloc(sizeof(player) * (players_number + computer_players_number));
	if(pl == NULL) {
		printf("Not Enough Ram!!!\n");
			exit(4);
	}
	for(i = 0; i < (players_number + computer_players_number); i++) {
		pl[i].number = i + 1;
		pl[i].r = ColorRow[i].r;
		pl[i].g = ColorRow[i].g;
		pl[i].b = ColorRow[i].b;
		pl[i].a = 0;
		if(i != players_number + computer_players_number - 1)
			pl[i].next = &pl[i + 1];
		else
			pl[i].next = &pl[0];
		if(i < players_number)
			pl[i].type = HUMAN;
		else
			pl[i].type = BOT_HARD;			
	}
	return pl;
}

void advance(board b, int i, int j, player *current, SDL_Renderer **ren) {
	int x, y, flag = FLAG_OFF, flag2 = FLAG_OFF, flag3 = FLAG_ON;
	int Game_terminator = FLAG_OFF;
	bucket buc, CTA;
	buffer B;
	binit(&buc);
	binit(&CTA);
	/*
	 *      Another Method to do this. Renderer not needed. Fast; But animation not supported.	
	 *	
	 *	if(b[i][j].balls == b[i][j].capacity) {
	 *		b[i][j].balls = 0;
	 *		b[i][j].p = NULL;
	 *		b[i][j].wave = wave;
	 *		if(i != 0) 
	 *			advance(b, i - 1, j, current);
	 *		if(i != rows - 1) 
	 *			advance(b, i + 1, j, current);
	 *		if(j != 0) 
	 *			advance(b, i, j - 1, current);
	 *		if(j != columns - 1) 
	 *			advance(b, i, j + 1, current);
	 *	}
	 *	else {
	 *		b[i][j].p = current;	
	 *		b[i][j].balls++;
	 *	}
	 */
	b[i][j].p = current; 
	b[i][j].balls++;
	while(flag3) {
		if(Game_terminator)
			break;
		else {
			flag = flag2 = FLAG_OFF;
			for(x = 0; x < rows && flag == FLAG_OFF; x++) {
				for(y = 0; y < columns && flag == FLAG_OFF; y++) {
					if(b[x][y].p != current && b[x][y].p != NULL)
						flag = FLAG_ON;
					if(b[x][y].balls == 3)
						flag2 = FLAG_ON;	
					if(x == (rows - 1) && y == (columns - 1) && flag2) 
						Game_terminator = FLAG_ON;	
				}
			if(b[rows - 1][columns - 1].p != current && b[rows - 1][columns - 1].p != NULL)
				Game_terminator = FLAG_OFF;			
			}		
			flag3 = FLAG_OFF;
			for(x = 0; x < rows; x++)
				for(y = 0; y < columns; y++) {
					if(b[x][y].balls > b[x][y].capacity) {
						flag3 = FLAG_ON;
						if(x != 0) {
							B.x = x - 1;
							B.y = y;
							addCell(&CTA, B);
						}	
						if(x != rows - 1) {
							B.x = x + 1;
							B.y = y;
							addCell(&CTA, B);
						}	
						if(y != 0) {
							B.x = x;
							B.y = y - 1;
							addCell(&CTA, B);
						}		
						if(y != columns - 1) {
							B.x = x;
							B.y = y + 1;
							addCell(&CTA, B);
						}
						B.x = x;
						B.y = y;
						addCell(&buc, B);
						b[x][y].balls -= (b[x][y].capacity + 1);
					}	
					if(b[x][y].balls == 0) 
						b[x][y].p = NULL;	
				}	
			AD_AnimateScreen(ren, b, current, &buc);		
			for(x = 0; x < storage(&CTA); x++) {
				B = check(&CTA, x);
				b[B.x][B.y].balls++;
				b[B.x][B.y].p = current;
			}
			destroyBucket(&buc);
			destroyBucket(&CTA);
		}	
	}	
}		

void Delete_Out_Players(board b, player **grid, player **current_player_address) {	
	player *temp, *temp2, *temp3;
	temp = temp2 = *grid;
	int i, j, count, flag = 0;
	while(temp2->next != *grid)
		temp2 = temp2->next;
	temp3 = temp2;	
	do {
		if(temp == temp3)
			flag++;
		for(i = 0, count = 0; (i < rows) && (count == 0); i++)
			for(j = 0; j < columns; j++)
				if(b[i][j].p == temp) {
					count++;
					break;	
				}		
		if(!(count)) {
			if(temp == *current_player_address)
				*current_player_address = (*current_player_address)->next;
			temp2->next = temp->next;
			if(temp == *grid)
				*grid = temp->next;
		}
		temp2 = temp;
		temp = temp->next;			
	} while(!(flag));
}	

void DestroyPlayer(player **pl_add) {
	free(*pl_add);
}

void DestroyBoard(board *b) {
	int i;
	for(i = 0; i < rows; i++)
		free((*b)[i]);
	free(*b);
}		

