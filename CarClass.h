#include<iostream>
#include"iGraphics.h"
using namespace std;
class Car{
private:
	double x;
	double y;
	double color;
public:
	Car(double a,double b,double clr){
		x=a;
		y=b;
		color=clr;
		double up_gx[10],up_gy[10],ls_gx[10],ls_gy[10],rs_gx[10],rs_gy[10],ds_gx[10],ds_gy[10];
	iSetColor(color,0,0);
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
	~Car(){
	}
};
