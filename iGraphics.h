# include <stdio.h>
# include <stdlib.h>
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glaux.lib")
#include "glut.h"
#include <time.h>
#include <math.h>
#include <windows.h>
#include <gl\glaux.h>

int iScreenHeight, iScreenWidth;
int iMouseX, iMouseY;
int ifft=0;
void (*iAnimFunction[10])(void)={0};
int iAnimCount=0;
int iAnimDelays[10];


void iDraw();
void iKeyboard(unsigned char);
void iSpecialKeyboard(unsigned char);
void iSpecialKeyboardUP(unsigned char);

void iMouseMove(int, int);
void iMouse(int button, int state, int x, int y);
void iPassiveMouseMove(int, int);

static void  __stdcall iA0(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[0]();}
static void  __stdcall iA1(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[1]();}
static void  __stdcall iA2(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[2]();}
static void  __stdcall iA3(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[3]();}
static void  __stdcall iA4(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[4]();}
static void  __stdcall iA5(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[5]();}
static void  __stdcall iA6(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[6]();}
static void  __stdcall iA7(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[7]();}
static void  __stdcall iA8(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[8]();}
static void  __stdcall iA9(HWND,unsigned int, unsigned int, unsigned long){iAnimFunction[9]();}

void iSettimer(int msec, void (*f)(void))
{
	int i = iAnimCount;
	
	if(iAnimCount>=10){printf("Error: Maximum number of already timer used.\n");return;}

	iAnimFunction[i] = f;
	iAnimDelays[i] = msec;
	
	if(iAnimCount == 0) SetTimer(0, 0, msec, iA0);
	if(iAnimCount == 1) SetTimer(0, 0, msec, iA1);
	if(iAnimCount == 2) SetTimer(0, 0, msec, iA2);
	if(iAnimCount == 3) SetTimer(0, 0, msec, iA3);
	if(iAnimCount == 4) SetTimer(0, 0, msec, iA4);

	if(iAnimCount == 5) SetTimer(0, 0, msec, iA5);
	if(iAnimCount == 6) SetTimer(0, 0, msec, iA6);
	if(iAnimCount == 7) SetTimer(0, 0, msec, iA7);
	if(iAnimCount == 8) SetTimer(0, 0, msec, iA8);
	if(iAnimCount == 9) SetTimer(0, 0, msec, iA9);
	iAnimCount++;
}

void iShowBMP(int x, int y, char filename[])
{
	AUX_RGBImageRec *TextureImage;

	TextureImage = auxDIBImageLoad(filename);
	
	glRasterPos2f(x, y);
	glDrawPixels(TextureImage->sizeX, TextureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
	
	free(TextureImage->data);
	free(TextureImage);

}

void iText(GLdouble x, GLdouble y, char *str, void* font=GLUT_BITMAP_8_BY_13)
{ 
   glRasterPos3d(x, y, 0);
   int i;
   for (i=0; str[i]; i++) {
      glutBitmapCharacter(font, str[i]); //,GLUT_BITMAP_8_BY_13, GLUT_BITMAP_TIMES_ROMAN_24
   }
}

void iPoint(double x, double y, int size=0)
{
	int i, j;
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	for(i=x-size;i<x+size;i++)
	{
		for(j=y-size; j<y+size;j++)
		{
			glVertex2f(i, j);
		}
	}
	glEnd();
}

void iLine(double x1, double y1, double x2, double y2)
{
	glBegin(GL_LINE_STRIP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void iFilledPolygon(double x[], double y[], int n)
{
	int i;
	if(n<3)return;
	glBegin(GL_POLYGON);
	for(i = 0; i < n; i++){
		glVertex2f(x[i], y[i]);
	}
	glEnd();		
}

void iPolygon(double x[], double y[], int n)
{
	int i;
	if(n<3)return;
	glBegin(GL_LINE_STRIP);
	for(i = 0; i < n; i++){
		glVertex2f(x[i], y[i]);
	}
	glVertex2f(x[0], y[0]);
	glEnd();		
}

void iRectangle(double left, double bottom, double dx, double dy)
{
	double x1, y1, x2, y2;
	
	x1 = left;
	y1 = bottom;
	x2=x1+dx;
	y2=y1+dy;

	iLine(x1, y1, x2, y1);
	iLine(x2, y1, x2, y2);
	iLine(x2, y2, x1, y2);
	iLine(x1, y2, x1, y1);
}

void iFilledRectangle(double left, double bottom, double dx, double dy)
{
	double xx[4], yy[4];
	double x1, y1, x2, y2;
	
	x1 = left;
	y1 = bottom;
	x2=x1+dx;
	y2=y1+dy;

	xx[0]=x1;
	yy[0]=y1;
	xx[1]=x2;
	yy[1]=y1;
	xx[2]=x2;
	yy[2]=y2;
	xx[3]=x1;
	yy[3]=y2;

	iFilledPolygon(xx, yy, 4);

}

void iFilledCircle(double x, double y, double r, int slices=100)
{
	double t, PI=acos(-1), dt, x1,y1, xp, yp;
	dt = 2*PI/(slices);
	xp = x+r;
	yp = y;
	glBegin(GL_POLYGON);
	for(t = 0; t <=2*PI; t+=dt)
	{
		x1 = x + r * cos(t);
		y1 = y + r * sin(t);

		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
	glEnd();
}
void iCircle(double x, double y, double r, int slices)
{
	double t, PI=acos(-1), dt, x1,y1, xp, yp;
	dt =2*PI/slices;
	xp = x+r;
	yp = y;
	for(t = 0; t <2*PI; t+=dt)
	{
		x1 = x + r * cos(t);
		y1 = y + r * sin(t);
		iLine(xp, yp, x1, y1);
		xp = x1;
		yp = y1;
	}
}

void iUpper_Arch(double x, double y, double r, int slices)
{
	double t, PI=acos(-1), dt, x1,y1, xp, yp;
	dt =2*PI/slices;
	xp = x+r*cos(58*PI/180);
	yp = y+r*sin(58*PI/180);
	glBegin(GL_POLYGON);
	for(t = 58*PI/180; t <122*PI/180; t+=dt)
	{
		x1 = x + r * cos(t);
		y1 = y + r * sin(t);
		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
	glEnd();
}
void iLower_Arch(double x, double y, double r, int slices)
{
	double t, PI=acos(-1), dt, x1,y1, xp, yp;
	dt =2*PI/slices;
	xp = x+r*cos(68.5*PI/180);
	yp = y+r*sin(68.5*PI/180);
	glBegin(GL_POLYGON);
	for(t = 68.5*PI/180; t <111.5*PI/180; t+=dt)
	{
		x1 = x + r * cos(t);
		y1 = y - r * sin(t);
		//iLine(xp, yp, x1, y1);
		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
	glEnd();
}

void iEllipse(double x, double y, double a, double b, int slices=100)
{
	double t, PI=acos(-1), dt, x1,y1, xp, yp;
	dt = 2*PI/slices;
	xp = x+a;
	yp = y;
	for(t = 0; t <=2*PI; t+=dt)
	{
		x1 = x + a * cos(t);
		y1 = y + b * sin(t);
		iLine(xp, yp, x1, y1);
		xp = x1;
		yp = y1;
	}
}
void iEllipse_Half(double x, double y, double a, double b, int slices=100)
{
	double t, PI=acos(-1), dt, x1,y1, xp, yp;
	dt = PI/slices;
	xp = x+a;
	yp = y;
	glBegin(GL_POLYGON);
	for(t =0; t <=PI; t+=dt)
	{
		x1 = x + a * cos(t);
		y1 = y + b * sin(t);
		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
		glEnd();
}
void iEllipse_LowerHalf(double x, double y, double a, double b, int slices=100)
{
	double t, PI=acos(-1), dt, x1,y1, xp, yp;
	dt = PI/slices;
	xp = x+a;
	yp = y;
	glBegin(GL_POLYGON);
	for(t =0; t <=PI; t+=dt)
	{
		x1 = x + a * cos(t);
		y1 = y - b * sin(t);
		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
		glEnd();
}

void iFilledEllipse(double x, double y, double a, double b, int slices=100)
{
	double t, PI=acos(-1), dt, x1,y1, xp, yp;
	dt =2*PI/slices;
	xp = x+a;
	yp = y;
	glBegin(GL_POLYGON);
	for(t = 0; t <=2*PI; t+=dt)
	{
		x1 = x + a * cos(t);
		y1 = y + b * sin(t);
		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
	glEnd();
}


void iSetColor(double r, double g, double b)
{
	glColor3f(r, g, b);
}

void iDelay(int sec)
{
	int t1, t2;
	t1 = time(0);
	while(1){
		t2 = time(0);
		if(t2-t1>=sec)
			break;
	}
}	


void iClear()
{
	glClear( GL_COLOR_BUFFER_BIT) ;
	glMatrixMode(GL_MODELVIEW) ;
	glClearColor(0,0,0,0);
		glFlush();
}

void displayFF(void){

	//iClear();
	iDraw();
	glutSwapBuffers() ;
}



void animFF(void)
{
	//iAnimation();
	if(ifft == 0){
		ifft = 1;
		iClear();
	}
	glutPostRedisplay();
}

void keyboardHandler1FF(unsigned char key, int x, int y)
{
	iKeyboard(key);
	glutPostRedisplay();
}
void keyboardHandler2FF(int key, int x, int y)
{
	iSpecialKeyboard(key);
	glutPostRedisplay();
}
void keyboardHandlerUP2FF(int key, int x, int y)
{
	iSpecialKeyboardUP(key);
	glutPostRedisplay();
}

void mouseMoveHandlerFF(int mx, int my)
{
	iMouseX = mx;
	iMouseY = iScreenHeight - my;
	iMouseMove(iMouseX, iMouseY);

	glFlush();
}

void mouseHandlerFF(int button, int state, int x, int y)
{
	iMouseX = x;
	iMouseY = iScreenHeight - y;

	iMouse(button, state, iMouseX, iMouseY);

	glFlush();
}
void mousePassiveMoveHandlerFF(int mx, int my)
{
	iMouseX = mx;
	iMouseY = iScreenHeight - my;
	iPassiveMouseMove(iMouseX, iMouseY);

	glFlush();
}

void iInitialize(int width=500, int height=500, char *title="iGraphics")
{
	iScreenHeight = height;
	iScreenWidth = width;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB) ;
	glutInitWindowSize(width , height ) ;
	glutInitWindowPosition( 10 , 10 ) ;
	glutCreateWindow(title) ;
	glClearColor( 0.0 , 0.0 , 0.0 , 0.0 ) ;
	glMatrixMode( GL_PROJECTION) ;
	glLoadIdentity() ;
	glOrtho(0.0 , width , 0.0 , height , -1.0 , 1.0) ;
	//glOrtho(-100.0 , 100.0 , -100.0 , 100.0 , -1.0 , 1.0) ;
	//SetTimer(0, 0, 10, timer_proc);
		
	iClear();

	glutDisplayFunc(displayFF) ;
	glutKeyboardFunc(keyboardHandler1FF);
	glutSpecialFunc(keyboardHandler2FF);
	glutSpecialUpFunc(keyboardHandlerUP2FF);
	glutMouseFunc(mouseHandlerFF);
	glutMotionFunc(mouseMoveHandlerFF);
	glutPassiveMotionFunc(mousePassiveMoveHandlerFF);
	glutIdleFunc(animFF) ;

	glutMainLoop() ;
}
void iCar_red(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(1,0,0);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(0,0,0);
	iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+9;	ds_gy[0]=y+20;
	ds_gx[1]=x+27;	ds_gy[1]=y+20;
	ds_gx[2]=x+30;	ds_gy[2]=y+15;
	ds_gx[3]=x+6;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,12,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iCar_black(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(0,0,0);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(1,1,1);
	/*iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);*/
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+10;	ds_gy[0]=y+20;
	ds_gx[1]=x+26;	ds_gy[1]=y+20;
	ds_gx[2]=x+29;	ds_gy[2]=y+15;
	ds_gx[3]=x+7;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,11,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iCar_white(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(1,1,1);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(0,0,0);
	iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+9;	ds_gy[0]=y+20;
	ds_gx[1]=x+27;	ds_gy[1]=y+20;
	ds_gx[2]=x+30;	ds_gy[2]=y+15;
	ds_gx[3]=x+6;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,12,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iPolice_Car(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	/*******CAR DESIGN******/
	iSetColor(0,0,0);
	iEllipse_Half(x+18,y+55,13,5,100);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iFilledRectangle(x+5,y+45,26,10);
	iFilledRectangle(x+5,y+5,26,5);
	iRectangle(x+10,y+27,15,3);
	iSetColor(1,1,1);
	//iText(x+10,y+50,"POLICE",GLUT_BITMAP_HELVETICA_5);
	iFilledRectangle(x+5,y+10,26,35);
	/*iSetColor(0,0,0);
	iRectangle(x+5,y+10,26,35);*/
	/*******LIGHTS**********/
	iSetColor(0,0,1);
	iFilledRectangle(x+10,y+25,5,3);
	iSetColor(0,0,0);
	iRectangle(x+15,y+25,5,3);
	iSetColor(1,0,0);
	iFilledRectangle(x+20,y+25,5,3);
	iSetColor(0,0,0);
	iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+9;	ds_gy[0]=y+20;
	ds_gx[1]=x+27;	ds_gy[1]=y+20;
	ds_gx[2]=x+30;	ds_gy[2]=y+15;
	ds_gx[3]=x+6;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,12,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);



}
void iCar_yellow(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(1,.8,0);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(0,0,0);
	/*iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);*/
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	iSetColor(0,0,0);
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+10;	ds_gy[0]=y+20;
	ds_gx[1]=x+26;	ds_gy[1]=y+20;
	ds_gx[2]=x+29;	ds_gy[2]=y+15;
	ds_gx[3]=x+7;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,11,4);
	iSetColor(0,0,0);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iCar_burnt(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(0,0,0);
	iEllipse_Half(x+18,y+55,13,5,100);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(1,.1,0);
	iFilledRectangle(x+5,y+5,26,50);	
	iSetColor(0,0,0);
	/*iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);*/
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	
	/***********GLASSES**********/
	iSetColor(1,1,1);
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+10;	ds_gy[0]=y+20;
	ds_gx[1]=x+26;	ds_gy[1]=y+20;
	ds_gx[2]=x+29;	ds_gy[2]=y+15;
	ds_gx[3]=x+7;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,11,4);
	iUpper_Arch(x+18,y+23,20,500);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iCar_green(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(0,.3,.1);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(0,0,0);
	iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+9;	ds_gy[0]=y+20;
	ds_gx[1]=x+27;	ds_gy[1]=y+20;
	ds_gx[2]=x+30;	ds_gy[2]=y+15;
	ds_gx[3]=x+6;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,12,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iCar_blue(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(0,0,.9);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(0,0,0);
	iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+9;	ds_gy[0]=y+20;
	ds_gx[1]=x+27;	ds_gy[1]=y+20;
	ds_gx[2]=x+30;	ds_gy[2]=y+15;
	ds_gx[3]=x+6;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,12,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iCar_sblue(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(0,1,1);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(0,0,0);
	iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+9;	ds_gy[0]=y+20;
	ds_gx[1]=x+27;	ds_gy[1]=y+20;
	ds_gx[2]=x+30;	ds_gy[2]=y+15;
	ds_gx[3]=x+6;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,12,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iCar_brown(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(.9,.4,.1);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(0,0,0);
	iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+9;	ds_gy[0]=y+20;
	ds_gx[1]=x+27;	ds_gy[1]=y+20;
	ds_gx[2]=x+30;	ds_gy[2]=y+15;
	ds_gx[3]=x+6;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,12,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}
void iCar_khaki(double x,double y)
{
	double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(.5,.4,.1);
	iEllipse_Half(x+18,y+55,13,5,100);
	iFilledRectangle(x+5,y+5,26,50);
	iEllipse_LowerHalf(x+18,y+5,13,5,100);
	iSetColor(0,0,0);
	iLine(x+14,y+60,x+7,y+40);
	iLine(x+22,y+60,x+29,y+40);
	iLine(x+10,y+1,x+10,y+15);
	iLine(x+25,y+1,x+25,y+15);
	iFilledRectangle(x+14,y+16,8,3);
	iUpper_Arch(x+18,y+23,20,500);
	/***********GLASSES**********/
	up_gx[0]=x+7;	up_gy[0]=y+40;
	up_gx[1]=x+29;	up_gy[1]=y+40;
	up_gx[2]=x+26;	up_gy[2]=y+32;
	up_gx[3]=x+10;	up_gy[3]=y+32;
	iFilledPolygon(up_gx,up_gy,4);
	ls_gx[0]=x+6;	ls_gy[0]=y+39;
	ls_gx[1]=x+9;	ls_gy[1]=y+32;
	ls_gx[2]=x+9;	ls_gy[2]=y+20;
	ls_gx[3]=x+6;	ls_gy[3]=y+15;
	iFilledPolygon(ls_gx,ls_gy,4);
	rs_gx[0]=x+30;	rs_gy[0]=y+39;
	rs_gx[1]=x+30;	rs_gy[1]=y+15;
	rs_gx[2]=x+27;	rs_gy[2]=y+20;
	rs_gx[3]=x+27;	rs_gy[3]=y+32;
	iFilledPolygon(rs_gx,rs_gy,4);
	ds_gx[0]=x+9;	ds_gy[0]=y+20;
	ds_gx[1]=x+27;	ds_gy[1]=y+20;
	ds_gx[2]=x+30;	ds_gy[2]=y+15;
	ds_gx[3]=x+6;	ds_gy[3]=y+15;
	iFilledPolygon(ds_gx,ds_gy,4);
	iEllipse_LowerHalf(x+18,y+15,12,4);
	iSetColor(1,1,1);
	iFilledEllipse(x+28,y+55,2,1);
	iFilledEllipse(x+8,y+55,2,1);
}



/*int main()
{
	iSetColor(1,1,1);
	iInitialize(600, 600);

	return 0 ;
}*/