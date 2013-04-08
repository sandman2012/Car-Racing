#include"CarClass.h"
#include<stdio.h>
#include<shellapi.h>
#include<windows.h>
#define yPosition 10
#define RED 1
int dvCo[]={0,90,180,270,360,450,540,630,720};
int rsCo[]={0,90,180,270,360,450,540,630,720};
int xPosition=250;

bool accelarate=false;
bool moveRight=false,moveLeft=false;

void initialize(){

}

void draw_road_side(){
	iSetColor(.14,.54,.14);
	iFilledRectangle(0,0,250,690);
	iFilledRectangle(550,0,250,690);
	iSetColor(0,0,0);
}


void draw_sand(){
	
	iSetColor(0,0,0);
	

}
void draw_main_road(){

	iSetColor(.4,.4,.4);
	iFilledRectangle(250,0,300,690);
	iSetColor(1,1,1);
	iFilledRectangle(250,0,5,690);
	iFilledRectangle(545,0,5,690);
	iSetColor(0,0,0);
	iRectangle(250,0,5,690);
	iRectangle(545,0,5,690);

			/*Road Devider*/
	if(accelarate==true){
		for(int i=0;i<9;i++){
			dvCo[i]-=5;
			if(dvCo[i]<0)dvCo[i]=720;
		}
	}
	for(int i=0;i<9;i++){
		iSetColor(1,1,1);
		iFilledRectangle(395,dvCo[i],5,30);
	}
			/*************/
	if(moveRight){
		xPosition+=1;
		Car ob(xPosition,yPosition,RED);
	}else if(moveLeft){
		xPosition-=1;
		Car ob(xPosition,yPosition,RED);
	}else{
		Car ob(xPosition,yPosition,RED);
	}


}
void iDraw(){
	iClear();
	draw_main_road();
	draw_road_side();
	draw_sand();

}


void iMouseMove(int mx, int my)
{
//place your codes here
}
/*
Function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN&&mx>=380&&mx<=636&&my>=466&&my<=530)
	{
	
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN&&mx>=380&&mx<=636&&my>=100&&my<=164)
	{
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
//place your codes here
	}
}

void iPassiveMouseMove(int mx, int my)
{
	//place your codes her
	
	
}

/*
Function iKeyboard() is called whenever the user hits a key in
keyboard. key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
	if(key == 'q')
	{
	
	}
	if(key=='e'){
		initialize();
	}

//place your codes for other keys here
}
/*
Function iSpecialKeyboard() is called whenver user hits special keys
like- function keys, home, end, pg up, pg down, arraows etc. you have
to use appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5,
GLUT_KEY_F6, GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10,
GLUT_KEY_F11, GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT,
GLUT_KEY_DOWN, GLUT_KEY_PAGE UP, GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME,
GLUT_KEY_END, GLUT_KEY_INSERT.
*/
void iSpecialKeyboard(unsigned char key)
{
if(key == GLUT_KEY_UP)
{
	accelarate=true;
}
if(key == GLUT_KEY_DOWN)
{


}
if(key == GLUT_KEY_RIGHT)
{	
	moveRight=true;
}
if(key == GLUT_KEY_LEFT)
{	
	moveLeft=true;
}


}
void iSpecialKeyboardUP(unsigned char key)
{

	if(GLUT_KEY_UP == key)
	{
		accelarate=false;
	}
	if(GLUT_KEY_DOWN == key)
	{
		
	}
	if(GLUT_KEY_LEFT ==key)
	{
		moveLeft=false;
	}
	if(GLUT_KEY_RIGHT == key)
	{
		moveRight=false;
	}
}

void iKeyboardUP(unsigned char key)
{

}

int main()
{
	
	iInitialize(800,690,"car racing");	
	return 0;

}
