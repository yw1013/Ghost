#include <stdlib.h>
#include <stdio.h>

#define OFFSET(x,y,z) (((x)*z)+(y))
#define SCANLINECOUNTER *(volatile u16*)0x4000006
#define RGB(r,g,b) (((r) & 0x1F) | (((g) & 0x1F) << 5) | (((b) & 0x1F) << 10))

typedef unsigned short u16;

u16 *videoBuffer = (u16*) 0x6000000;

// A function to set pixel (r, c) to the color passed in.
void setPixel(int r, int c, u16 color) {
	videoBuffer[OFFSET(r, c, 240)] = color;
}


// A function to draw a FILLED rectangle starting at (r, c)
void drawRect(int r, int c, int width, int height, u16 color){ 
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			setPixel(r + i, c + j, color);
		}
	}
}


// A function to draw a HOLLOW rectangle starting at (r, c)
// NOTE: It has to run in O(w+h) time.
void drawHollowRect(int r, int c, int width, int height, u16 color){
	for(int i = r; i < height+r; i++){
		setPixel(i,c,color);
		setPixel(i, c+width-1, color);
	}
	for(int i = c; i < width+c; i++){
		setPixel(r,i,color);
		setPixel(r+height-1, i, color);			
	}
}


/* drawimage3
   * A function that will draw an arbitrary sized image
   * onto the screen
   * @param r row to draw the image
   * @param c column to draw the image
   * @param width width of the image
   * @param height height of the image
   * @param image Pointer to the first element of the image.
   */
void drawImage3(int r, int c, int width, int height, const u16* image){
	for (int i = r; i < width + r; i++) {
		for (int j = c; j < height + c; j++){
			videoBuffer[j * 240 + i] = image[(j-c) * width + (i-r)];
		}
	}
}


void waitForVBlank(){
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}



