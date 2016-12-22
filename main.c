#include <stdio.h>
#include <stdlib.h>
#include "start.h"
#include "over.h"
#include "win.h"
#include "house.h"
#include "ghost.h"
#include "boy.h"

#define REG_DISPCNT *(unsigned short*) 0x4000000
#define COLOR(R,G,B) (((R) & 0x1F) | (((G) & 0x1F) << 5) | (((B) & 0x1F) << 10))
#define BLACK   COLOR(0 , 0 , 0 )
#define MODE_3 3
#define BG2_EN (1 << 10)
#define BUTTONS       *(volatile unsigned short*) 0x4000130
#define BUTTON_A      (1 << 0)
#define BUTTON_B      (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START  (1 << 3)
#define BUTTON_RIGHT  (1 << 4)
#define BUTTON_LEFT   (1 << 5)
#define BUTTON_UP     (1 << 6)
#define BUTTON_DOWN   (1 << 7)
#define BUTTON_R      (1 << 8)
#define BUTTON_L      (1 << 9)
#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

typedef unsigned short u16;	

typedef struct {
	double col;
	double row;
} Character;

Character player = {10, 145};
Character home = {220, 80};

typedef struct {
	double col;
	double row;
	double moving;
} Ghost;

Ghost ghost1 = {0, 5, 1};
Ghost ghost2 = {225, 20, -1};
Ghost ghost3 = {0, 35, 1};
Ghost ghost4 = {225, 50, -1};
Ghost ghost5 = {0, 65, 1.2};
Ghost ghost6 = {225, 80, -1};
Ghost ghost7 = {0, 95, 1};
Ghost ghost8 = {225, 110, -1};
Ghost ghost9 = {0, 125, 1};
Ghost ghost10 = {225, 140, -1};

void setPixel(int r, int c, u16 color);
void drawRect(int r, int c, int width, int height, u16 color);
void drawHollowRect(int r, int c, int width, int height, u16 color);
void drawImage3(int r, int c, int width, int height, const u16* image);
void waitForVBlank();

void drawPlayer();
int movePlayer();
void moveGhost1();
void moveGhosts();

enum STATE {
	START,
	PLAY,
	OVER,
	WIN
};

enum STATE state = START;

int main(){
	REG_DISPCNT = MODE_3 | BG2_EN;
	int state = START;
	while(1){
		waitForVBlank();
		switch(state){
		case START:
			drawImage3(0, 0, 240, 160, start);
			player.col = 10;
			player.row = 145;

			ghost1.col = 0;
			ghost1.row = 5;
			ghost1.moving = 1;

			ghost2.col = 225;
			ghost2.row = 20;
			ghost2.moving = -1;

			ghost3.col = 0;
			ghost3.row = 35;
			ghost3.moving = 1;

			ghost4.col = 225;
			ghost4.row = 50;
			ghost4.moving = -1;

			ghost5.col = 0;
			ghost5.row = 65;
			ghost5.moving = 1;

			ghost6.col = 225;
			ghost6.row = 80;
			ghost6.moving = -1;

			ghost7.col = 0;
			ghost7.row = 95;
			ghost7.moving = 1;

			ghost8.col = 225;
			ghost8.row = 110;
			ghost8.moving = -1;

			ghost9.col = 0;
			ghost9.row = 125;
			ghost9.moving = 1;

			ghost10.col = 225;
			ghost10.row = 140;
			ghost10.moving = -1;

			
			if (KEY_DOWN_NOW(BUTTON_START)){
				state = PLAY;
			}
			break;

		case PLAY:
			drawRect(0, 0, 240, 160, BLACK) ;
			drawImage3(220, 0, HOUSE_WIDTH, HOUSE_HEIGHT, house);
			drawPlayer();
			state = movePlayer();

			break;

		case OVER:
			drawImage3(0,0, 240, 160, over);
			if(KEY_DOWN_NOW(BUTTON_SELECT)){
				state = START;
			}
			break;

		case WIN:
			drawImage3(0,0, 240, 160, win);
			if(KEY_DOWN_NOW(BUTTON_SELECT)){
				state = START;
			}
			break;
		}
	}
	return 0;
			
}

void drawPlayer(){
	if(player.col < 0){
		player.col = 0;
	} else if(player.col > 225){
		player.col = 225;
	}
	if(player.row < 0){
		player.row = 0;
	} else if(player.row > 145){
		player.row = 145;
	}
	drawImage3(player.col, player.row, BOY_WIDTH, BOY_HEIGHT, boy);
}

int movePlayer(){
	drawPlayer();
	while(1){
		if(KEY_DOWN_NOW(BUTTON_SELECT)){
			return START;
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT)){
			player.col = player.col -1;
			drawPlayer();
		}	
		if(KEY_DOWN_NOW(BUTTON_RIGHT)){
			player.col = player.col +1;
			drawPlayer();
		}
		if(KEY_DOWN_NOW(BUTTON_UP)){
			player.row = player.row -1;
			drawPlayer();
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN)){
			player.row = player.row +1;
			drawPlayer();
		}
		moveGhost1();
		moveGhosts();

		if(
		((((player.col-10 <= ghost1.col)&&(player.col+10 >= ghost1.col)) && ((player.row-10 <= ghost1.row)&&(player.row+10 >= ghost1.row)))) || 
		((((player.col-10 <= ghost1.col)&&(player.col+10 >= ghost1.col+15)) && ((player.row-10 <= ghost1.row)&&(player.row+10 >= ghost1.row)))) ||
            	((((player.col-10 <= ghost1.col)&&(player.col+10 >= ghost1.col)) && ((player.row-10 <= ghost1.row)&&(player.row+10 >= ghost1.row+15)))) ||
            	((((player.col-10 <= ghost1.col)&&(player.col+10 >= ghost1.col+15)) && ((player.row-10 <= ghost1.row)&&(player.row+10 >= ghost1.row+15)))) ||

		((((player.col-10 <= ghost2.col)&&(player.col+10 >= ghost2.col)) && ((player.row-10 <= ghost2.row)&&(player.row+10 >= ghost2.row)))) || 
		((((player.col-10 <= ghost2.col)&&(player.col+10 >= ghost2.col+15)) && ((player.row-10 <= ghost2.row)&&(player.row+10 >= ghost2.row)))) ||
            	((((player.col-10 <= ghost2.col)&&(player.col+10 >= ghost2.col)) && ((player.row-10 <= ghost2.row)&&(player.row+10 >= ghost2.row+15)))) ||
            	((((player.col-10 <= ghost2.col)&&(player.col+10 >= ghost2.col+15)) && ((player.row-10 <= ghost2.row)&&(player.row+10 >= ghost2.row+15)))) ||

		((((player.col-10 <= ghost3.col)&&(player.col+10 >= ghost3.col)) && ((player.row-10 <= ghost3.row)&&(player.row+10 >= ghost3.row)))) || 
		((((player.col-10 <= ghost3.col)&&(player.col+10 >= ghost3.col+15)) && ((player.row-10 <= ghost3.row)&&(player.row+10 >= ghost3.row)))) ||
            	((((player.col-10 <= ghost3.col)&&(player.col+10 >= ghost3.col)) && ((player.row-10 <= ghost3.row)&&(player.row+10 >= ghost3.row+15)))) ||
            	((((player.col-10 <= ghost3.col)&&(player.col+10 >= ghost3.col+15)) && ((player.row-10 <= ghost3.row)&&(player.row+10 >= ghost3.row+15)))) ||

		((((player.col-10 <= ghost4.col)&&(player.col+10 >= ghost4.col)) && ((player.row-10 <= ghost4.row)&&(player.row+10 >= ghost4.row)))) || 
		((((player.col-10 <= ghost4.col)&&(player.col+10 >= ghost4.col+15)) && ((player.row-10 <= ghost4.row)&&(player.row+10 >= ghost4.row)))) ||
            	((((player.col-10 <= ghost4.col)&&(player.col+10 >= ghost4.col)) && ((player.row-10 <= ghost4.row)&&(player.row+10 >= ghost4.row+15)))) ||
            	((((player.col-10 <= ghost4.col)&&(player.col+10 >= ghost4.col+15)) && ((player.row-10 <= ghost4.row)&&(player.row+10 >= ghost4.row+15)))) ||

		((((player.col-10 <= ghost5.col)&&(player.col+10 >= ghost5.col)) && ((player.row-10 <= ghost5.row)&&(player.row+10 >= ghost5.row)))) || 
		((((player.col-10 <= ghost5.col)&&(player.col+10 >= ghost5.col+15)) && ((player.row-10 <= ghost5.row)&&(player.row+10 >= ghost5.row)))) ||
            	((((player.col-10 <= ghost5.col)&&(player.col+10 >= ghost5.col)) && ((player.row-10 <= ghost5.row)&&(player.row+10 >= ghost5.row+15)))) ||
            	((((player.col-10 <= ghost5.col)&&(player.col+10 >= ghost5.col+15)) && ((player.row-10 <= ghost5.row)&&(player.row+10 >= ghost5.row+15)))) ||

		((((player.col-10 <= ghost6.col)&&(player.col+10 >= ghost6.col)) && ((player.row-10 <= ghost6.row)&&(player.row+10 >= ghost6.row)))) || 
		((((player.col-10 <= ghost6.col)&&(player.col+10 >= ghost6.col+15)) && ((player.row-10 <= ghost6.row)&&(player.row+10 >= ghost6.row)))) ||
            	((((player.col-10 <= ghost6.col)&&(player.col+10 >= ghost6.col)) && ((player.row-10 <= ghost6.row)&&(player.row+10 >= ghost6.row+15)))) ||
            	((((player.col-10 <= ghost6.col)&&(player.col+10 >= ghost6.col+15)) && ((player.row-10 <= ghost6.row)&&(player.row+10 >= ghost6.row+15)))) ||

		((((player.col-10 <= ghost7.col)&&(player.col+10 >= ghost7.col)) && ((player.row-10 <= ghost7.row)&&(player.row+10 >= ghost7.row)))) || 
		((((player.col-10 <= ghost7.col)&&(player.col+10 >= ghost7.col+15)) && ((player.row-10 <= ghost7.row)&&(player.row+10 >= ghost7.row)))) ||
            	((((player.col-10 <= ghost7.col)&&(player.col+10 >= ghost7.col)) && ((player.row-10 <= ghost7.row)&&(player.row+10 >= ghost7.row+15)))) ||
            	((((player.col-10 <= ghost7.col)&&(player.col+10 >= ghost7.col+15)) && ((player.row-10 <= ghost7.row)&&(player.row+10 >= ghost7.row+15)))) ||

		((((player.col-10 <= ghost8.col)&&(player.col+10 >= ghost8.col)) && ((player.row-10 <= ghost8.row)&&(player.row+10 >= ghost8.row)))) || 
		((((player.col-10 <= ghost8.col)&&(player.col+10 >= ghost8.col+15)) && ((player.row-10 <= ghost8.row)&&(player.row+10 >= ghost8.row)))) ||
            	((((player.col-10 <= ghost8.col)&&(player.col+10 >= ghost8.col)) && ((player.row-10 <= ghost8.row)&&(player.row+10 >= ghost8.row+15)))) ||
            	((((player.col-10 <= ghost8.col)&&(player.col+10 >= ghost8.col+15)) && ((player.row-10 <= ghost8.row)&&(player.row+10 >= ghost8.row+15)))) ||

		((((player.col-10 <= ghost9.col)&&(player.col+10 >= ghost9.col)) && ((player.row-10 <= ghost9.row)&&(player.row+10 >= ghost9.row)))) || 
		((((player.col-10 <= ghost9.col)&&(player.col+10 >= ghost9.col+15)) && ((player.row-10 <= ghost9.row)&&(player.row+10 >= ghost9.row)))) ||
            	((((player.col-10 <= ghost9.col)&&(player.col+10 >= ghost9.col)) && ((player.row-10 <= ghost9.row)&&(player.row+10 >= ghost9.row+15)))) ||
            	((((player.col-10 <= ghost9.col)&&(player.col+10 >= ghost9.col+15)) && ((player.row-10 <= ghost9.row)&&(player.row+10 >= ghost9.row+15)))) ||

		((((player.col-10 <= ghost10.col)&&(player.col+10 >= ghost10.col)) && ((player.row-10 <= ghost10.row)&&(player.row+10 >= ghost10.row)))) || 
		((((player.col-10 <= ghost10.col)&&(player.col+10 >= ghost10.col+15)) && ((player.row-10 <= ghost10.row)&&(player.row+10 >= ghost10.row)))) ||
           	((((player.col-10 <= ghost10.col)&&(player.col+10 >= ghost10.col)) && ((player.row-10 <= ghost10.row)&&(player.row+10 >= ghost10.row+15)))) ||
            	((((player.col-10 <= ghost10.col)&&(player.col+10 >= ghost10.col+15)) && ((player.row-10 <= ghost10.row)&&(player.row+10 >= ghost10.row+15))))){
			return OVER;
		}


		if(
		((((player.col-10 <= home.col)&&(player.col+10 >= home.col)) && ((player.row+70 <= home.row)&&(player.row+80 >= home.row)))) || 
		((((player.col-10 <= home.col)&&(player.col+10 >= home.col+15)) && ((player.row+70 <= home.row)&&(player.row+80 >= home.row)))) ||
           	((((player.col-10 <= home.col)&&(player.col+10 >= home.col)) && ((player.row+70 <= home.row)&&(player.row+80 >= home.row+15)))) ||
            	((((player.col-10 <= home.col)&&(player.col+10 >= home.col+15)) && ((player.row+70 <= home.row)&&(player.row+80 >= home.row+15))))){
			return WIN;
		}

	}
	return 0;
}


void moveGhost1(){
	drawImage3(ghost1.col, ghost1.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	drawImage3(ghost10.col, ghost10.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

	if ((ghost1.col > 0 && ghost1.col < 5) && ghost1.moving <= 0) {
		ghost1.moving = -ghost1.moving;
		ghost10.moving = -ghost10.moving;
	}
	if(ghost1.col >= 200 && ghost1.col <= 205){
		ghost1.moving = -ghost1.moving;
		ghost1.col = ghost1.col + ghost1.moving;
		drawImage3(ghost1.col, ghost1.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost10.moving = -ghost10.moving;
		ghost10.col = ghost10.col + ghost10.moving;
		drawImage3(ghost10.col, ghost10.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

	}else if(ghost1.col >= 0 && ghost1.col <= 200){
		ghost1.moving = ghost1.moving;
		ghost1.col = ghost1.col + ghost1.moving;
		drawImage3(ghost1.col, ghost1.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost10.moving = ghost10.moving;
		ghost10.col = ghost10.col + ghost10.moving;
		drawImage3(ghost10.col, ghost10.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	}
}

void moveGhosts(){
	drawImage3(ghost3.col, ghost3.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	drawImage3(ghost5.col, ghost5.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	drawImage3(ghost7.col, ghost7.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	drawImage3(ghost9.col, ghost9.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

	drawImage3(ghost2.col, ghost2.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	drawImage3(ghost4.col, ghost4.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	drawImage3(ghost6.col, ghost6.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	drawImage3(ghost8.col, ghost8.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

	if ((ghost3.col > 0 && ghost3.col < 3) && ghost3.moving <= 0) {
		ghost3.moving = -ghost3.moving;
		ghost5.moving = -ghost5.moving;
		ghost7.moving = -ghost7.moving;
		ghost9.moving = -ghost9.moving;

		ghost2.moving = -ghost2.moving;
		ghost4.moving = -ghost4.moving;
		ghost6.moving = -ghost6.moving;
		ghost8.moving = -ghost8.moving;
	}
	if(ghost3.col >= 225 && ghost3.col <= 235){
		ghost3.moving = -ghost3.moving;
		ghost3.col = ghost3.col + ghost3.moving;
		drawImage3(ghost3.col, ghost3.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost5.moving = -ghost5.moving;
		ghost5.col = ghost5.col + ghost5.moving;
		drawImage3(ghost5.col, ghost5.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost7.moving = -ghost7.moving;
		ghost7.col = ghost7.col + ghost7.moving;
		drawImage3(ghost7.col, ghost7.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost9.moving = -ghost9.moving;
		ghost9.col = ghost9.col + ghost9.moving;
		drawImage3(ghost9.col, ghost9.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost2.moving = -ghost2.moving;
		ghost2.col = ghost2.col + ghost2.moving;
		drawImage3(ghost2.col, ghost2.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost4.moving = -ghost4.moving;
		ghost4.col = ghost4.col + ghost4.moving;
		drawImage3(ghost4.col, ghost4.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost6.moving = -ghost6.moving;
		ghost6.col = ghost6.col + ghost6.moving;
		drawImage3(ghost6.col, ghost6.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost8.moving = -ghost8.moving;
		ghost8.col = ghost8.col + ghost8.moving;
		drawImage3(ghost8.col, ghost8.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

	}else if(ghost3.col >= 0 && ghost3.col <= 225){
		ghost3.moving = ghost3.moving;
		ghost3.col = ghost3.col + ghost3.moving;
		drawImage3(ghost3.col, ghost3.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost5.moving = ghost5.moving;
		ghost5.col = ghost5.col + ghost5.moving;
		drawImage3(ghost5.col, ghost5.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost7.moving = ghost7.moving;
		ghost7.col = ghost7.col + ghost7.moving;
		drawImage3(ghost7.col, ghost7.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost9.moving = ghost9.moving;
		ghost9.col = ghost9.col + ghost9.moving;
		drawImage3(ghost9.col, ghost9.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);


		ghost2.moving = ghost2.moving;
		ghost2.col = ghost2.col + ghost2.moving;
		drawImage3(ghost2.col, ghost2.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost4.moving = ghost4.moving;
		ghost4.col = ghost4.col + ghost4.moving;
		drawImage3(ghost4.col, ghost4.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost6.moving = ghost6.moving;
		ghost6.col = ghost6.col + ghost6.moving;
		drawImage3(ghost6.col, ghost6.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);

		ghost8.moving = ghost8.moving;
		ghost8.col = ghost8.col + ghost8.moving;
		drawImage3(ghost8.col, ghost8.row, GHOST_WIDTH, GHOST_HEIGHT, ghost);
	}
}


