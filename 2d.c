#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.14159

//adding a player
/*
	px  = player X position
	py  = player Y position
*/
float px, py; //player position

void drawPlayer() {
	glColor3f(1,1,0); //yellow-colored player
	glPointSize(8); //size of the player
	glBegin(GL_POINTS);
	glVertex2i(px,py);
	glEnd();
	
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(px,py);
	glEnd();
}

//adding the map
int mapX=8, mapY=8, mapS=64;
int map[] = {
	1,1,1,1,1,1,1,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,1,1,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
};
void drawMap2D() {
	int x,y,xo,yo;
	for(y=0;y<mapY;y++) {
		for(x=0;x<mapX;x++) {
			if(map[y*mapX+x]==1) {glColor3f(1,1,1);} //map color
			else {glColor3f(0,0,0);} //empty space color
			xo=x*mapS; yo=y*mapS;
			glBegin(GL_QUADS);
			//+1 and -1 values adds lines in the map
			glVertex2i(xo     +1, yo     +1);
			glVertex2i(xo     +1, yo+mapS-1);
			glVertex2i(xo+mapS-1, yo+mapS-1);
			glVertex2i(xo+mapS-1, yo     +1);
			glEnd();
			
			
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawPlayer();
	glutSwapBuffers();
}

void buttons(unsigned char key, int x, int y) {
	if(key=='a') {px-=5;}
	if(key=='d') {px+=5;}
	if(key=='w') {py-=5;}
	if(key=='s') {py+=5;}
	glutPostRedisplay();
}

void init() {
	glClearColor(0.3,0.3,0.3,0); //dark gray color bg
	gluOrtho2D(0,1024,512,0); //1024x512 pixels window dimension
	px=300; py=300;
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 512);
	glutCreateWindow("Raycaster!!!");
	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);
	init();
	glutMainLoop();
}
