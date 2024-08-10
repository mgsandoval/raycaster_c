#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.14159
#define P2 PI/2
#define P3 3*PI/2

/*
~|---* Variables overview *---|~
	px  = player X position
	py  = player Y position
	pdx = player delta X position
	pdy = player delta Y position
	pa  = player angle
*/

//adding a player
float px, py, pdx,pdy,pa; //player position

void drawPlayer() {
	glColor3f(1,1,0); //yellow-colored player
	glPointSize(8); //size of the player
	glBegin(GL_POINTS);
	glVertex2i(px,py);
	glEnd();
	
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(px,py);
	glVertex2i(px+pdx*5, py+pdy*5);
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
	1,0,0,0,0,1,0,1,
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
//adding rays
void drawRays2D() {
	int r, mx, my, mp, dof; float rx, ry, ra,xo,yo;
	ra=pa;
	for(r=0;r<1;r++) {
		//---Check horizontal lines---
		/*dof=0;
		float aTan=-1/tan(ra);
		if(ra>PI){ry=(((int)py>>6)<<6)-0.0001; rx=(py-ry)*aTan+px; yo=-64; xo=-yo*aTan;} //looking down
		if(ra<PI){ry=(((int)py>>6)<<6)+64;     rx=(py-ry)*aTan+px; yo= 64; xo=-yo*aTan;} //looking up
		if(ra==0 || ra==PI){rx=px; ry=py; dof=8;} //looking straight left or right
		while(dof<8) {
			mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
			if(mp<mapX*mapY && map[mp]==1){dof=8;} //hit wall
			else {rx+=xo; ry+=yo; dof+=1;} //next line
		}
		glColor3f(0,1,0); //beam color
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px,py);
		glVertex2i(rx,ry);
		glEnd();*/
		
		//---Check vertical lines---
		dof=0;
		float aTan=-1/tan(ra);
		if(ra>PI){ry=(((int)py>>6)<<6)-0.0001; rx=(py-ry)*aTan+px; yo=-64; xo=-yo*aTan;} //looking left
		if(ra<PI){ry=(((int)py>>6)<<6)+64;     rx=(py-ry)*aTan+px; yo= 64; xo=-yo*aTan;} //looking right
		if(ra==0 || ra==PI){rx=px; ry=py; dof=8;} //looking straight up and down
		while(dof<8) {
			mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
			if(mp<mapX*mapY && map[mp]==1){dof=8;} //hit wall
			else {rx+=xo; ry+=yo; dof+=1;} //next line
		}
		glColor3f(0,1,0); //beam color
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px,py);
		glVertex2i(rx,ry);
		glEnd();
	}
}

//Print in-screen
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawPlayer();
	drawRays2D();
	glutSwapBuffers();
}

//Assign keyboard buttons
void buttons(unsigned char key, int x, int y) {
//	if(key=='a') {px-=5;}
//	if(key=='d') {px+=5;}
//	if(key=='w') {py-=5;}
//	if(key=='s') {py+=5;}
	if(key=='a') {pa-=0.1; if(pa<   0) {pa+=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;}
	if(key=='d') {pa+=0.1; if(pa>2*PI) {pa-=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;}
	if(key=='w') {px+=pdx; py+=pdy;}
	if(key=='s') {px-=pdx; py-=pdy;}
	glutPostRedisplay();
}

void init() {
	glClearColor(0.3,0.3,0.3,0); //dark gray color bg
	gluOrtho2D(0,1024,512,0); //1024x512 pixels window dimension
	px=300; py=300; pdx=cos(pa)*5; pdy=sin(pa)*5;
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
