#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.14159
#define P2 PI/2 //90 degrees
#define P3 3*PI/2 //270 degrees
#define DR 0.0174533 //one degree in radians

/*
~|---* Variables overview *---|~
	float px  				= player X position
	float py  				= player Y position
	float pdx 				= player delta X position
	float pdy 				= player delta Y position
	float pa  				= player angle
	int mapX  				= map X width
	int mapY  				= map Y height
	int mapS  				= map chunk/tile size
	int map[] 				= array which stores the map world in-screen
	int x	  				= used in the for loop for drawing the X axis of tiles
	int y	  				= used in the for loop for drawing the Y axis of tiles
	int xo	  				= used for drawing borders in tiles
	int yo	  				= used for drawing borders in tiles
	int r	  				= ray
	int mx	  				= map X position, and where the ray will hit the wall
	int my	  				= map Y position, and where the ray will hit the wall
	int mp	  				= map position
	int dof	  				= verifies if the ray touches a wall, if it touches one, finishes the ray
	float rx  				= ray X position
	float ry  				= ray Y position
	float ra  				= ray angle
	float xo  				= used for calculating the next X offset
	float yo				= used for calculating the next Y offset
	float disH=1000000 		= horizontal distance between the player and the walls
	float hx=px				= horizontal rays X position, equals to the value of player X position
	float hy=py				= horizontal rays Y position, equals to the value of player Y position
	float aTan=-1/tan(ra)	= inverse tangent, equals to -1/tan(ray angle), calculates the ray's nearest horizontal line
	float disV=1000000		= vertical distance between the player and the walls
	float vx=px				= vertical rays X position, equals to the value of player X position
	float vy=py				= vertical rays Y position, equals to the value of player Y position
	float nTan=-tan(ra)		= negative tangent, gets the value of the raeangle
	disT					= final distance
	lineH					= line height, 3D window size
	lineO					= line offset
	ca						= cosine of angle
*/

//adding a player
float px, py, pdx,pdy,pa; //player position

void drawPlayer() {
	glColor3f(1,1,0); 	//player color
	glPointSize(8); 	//size of the player
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
			if(map[y*mapX+x]==1) {glColor3f(1,1,1);}	//map color
			//else if (map[y*mapX+x]==2){glColor3f(0,1,0);} //green tile test
			else {glColor3f(0,0,0);} 					//empty space color
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

//returns the distance between the player and the rays endpoint
float dist(float ax, float ay, float bx, float by, float ang) {
	return (sqrt((bx-ax) * (bx-ax) + (by-ay) * (by-ay))); //Pythagorean theorem, ray stops in each wall
}

//adding rays
void drawRays2D() {
	int r, mx, my, mp, dof; float rx, ry, ra,xo,yo,disT;
	ra=pa-DR*30; if(ra<0){ra+=2*PI;} if(ra>2*PI){ra-=2*PI;} //adds the angle of rays, if ra=pa-DR*360 and the for is r<360 you'll have an illumination system
	for(r=0;r<60;r++) { //number of rays
		//---Check horizontal lines---
		dof=0;
		float disH=1000000, hx=px, hy=py;
		float aTan=-1/tan(ra);
		if(ra>PI){ry=(((int)py>>6)<<6)-0.0001; rx=(py-ry)*aTan+px; yo=-64; xo=-yo*aTan;} //if ray's looking down
		if(ra<PI){ry=(((int)py>>6)<<6)+64;     rx=(py-ry)*aTan+px; yo= 64; xo=-yo*aTan;} //if ray's looking up
		if(ra==0 || ra==PI){rx=px; ry=py; dof=8;}	//looking straight left or right
		while(dof<8) {
			mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
			if(mp>0 && mp<mapX*mapY && map[mp]>0){hx=rx; hy=ry; disH=dist(px,py,hx,hy,ra); dof=8;} //hit wall
			else {rx+=xo; ry+=yo; dof+=1;} 			//next line
		}
		//glColor3f(0,1,0); //beam color (r,g,b)
//		glLineWidth(10); //beam width
//		glBegin(GL_LINES);
//		glVertex2i(px,py);
//		glVertex2i(rx,ry);
//		glEnd();
		
		//---Check vertical lines---
		dof=0;
		float disV=1000000, vx=px, vy=py;
		float nTan=-tan(ra);
		if(ra>P2 && ra<P3){rx=(((int)px>>6)<<6)-0.0001; ry=(px-rx)*nTan+py; xo=-64; yo=-xo*nTan;} 	//looking left
		if(ra<P2 || ra>P3){rx=(((int)px>>6)<<6)+64;     ry=(px-rx)*nTan+py; xo= 64; yo=-xo*nTan;} 	//looking right
		if(ra==0 || ra==PI){rx=px; ry=py; dof=8;} 													//looking straight up and down
		while(dof<8) {
			mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
			if(mp>0 && mp<mapX*mapY && map[mp]>0){vx=rx; vy=ry; disV=dist(px,py,vx,vy,ra); dof=8;} //hit wall
			else {rx+=xo; ry+=yo; dof+=1;} //next line
		}
		if(disV<disH) {rx=vx; ry=vy; ry=vy; disT=disV; glColor3f(0.9,0,0);}			//Vertical wall hit
		if(disH<disV) {rx=hx; ry=hy; ry=hy; disT=disH; glColor3f(0.7,0,0);}			//Horizontal wall hit
		
		//---Draw 3D walls---
		float ca=pa-ra; if(ca<0){ca+=2*PI;} if(ca>2*PI){ca-=2*PI;} disT=disT*cos(ca); 	//fix fisheye
		float lineH=(mapS*320)/disT; if(lineH>320){lineH=320;} 							//line height
		float lineO=160-lineH/2; 														//line offset
		glLineWidth(8); glBegin(GL_LINES); glVertex2i(r*8+530,lineO); glVertex2i(r*8+530,lineH+lineO); glEnd();
		ra+=DR; if(ra<0){ra+=2*PI;} if(ra>2*PI){ra-=2*PI;}
		
		glColor3f(1,0,0); 	//beam color (r,g,b)
		glLineWidth(3); 	//beam width
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
	drawRays2D();
	drawPlayer();
	glutSwapBuffers();
}

//Assign keyboard buttons to make player move
void buttons(unsigned char key, int x, int y) {
//	if(key=='a') {px-=5;}
//	if(key=='d') {px+=5;}
//	if(key=='w') {py-=5;}
//	if(key=='s') {py+=5;}
	if(key=='a') {pa-=0.1; if(pa<   0) {pa+=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;} //player angle and speed
	if(key=='d') {pa+=0.1; if(pa>2*PI) {pa-=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;}
	if(key=='w') {px+=pdx; py+=pdy;}
	if(key=='s') {px-=pdx; py-=pdy;}
	glutPostRedisplay();
}

void init() {
	glClearColor(0.3,0.3,0.3,0); 	//dark gray color background
	gluOrtho2D(0,1024,512,0); 		//1024x512 pixels window dimension
	px=300; py=300; pdx=cos(pa)*5; pdy=sin(pa)*5; //initializes player position
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
