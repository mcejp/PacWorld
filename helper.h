#ifndef HELPER_H
#define HELPER_H

////////Watson's Little Helper-Functions/////////////////
////////helper.h/////////////////////////////////////////

////Use to produce random integer numbers////////
////in the range of 0 ... i//////////////////////

inline int Random(int i)
{
	static bool firsttime=true;

	//Seeed generator with system time
	if(firsttime==true)
	{
		firsttime=false;

		srand( (unsigned)time( NULL ) );

		int x=Random(10);
	}

	int output=rand()%i;

	return output;	
}

////Use to convert integers to CStrings//////////

inline CString IntToStr(int i)
{	
	char ch[10];			

	_itoa(i, ch, 10);
			
	CString output=ch;	

	return output;
}

/////Draws circle with given center and diameter/////

inline void Circle(CDC &dc, int x,int y,int d)
{		
	int xm=x-(d/2);
	int ym=y-(d/2);
	
	dc.Ellipse(xm,ym,xm+d,ym+d);
}

/////Draws good visible text to debug program with//////

inline void DebugText(CDC &dc, int x,int y, CString s)
{	
	CFont font;
	font.CreatePointFont(80,"MS Sans Serif",&dc);

	CFont *pOldFont=dc.SelectObject(&font);

	dc.SetBkMode(TRANSPARENT);		

	dc.SetTextColor(RGB(0,0,0));
		dc.TextOut(x+1,y+1,s);	

	dc.SetTextColor(RGB(255,255,255));
		dc.TextOut(x,y,s);	
		
	dc.SelectObject(pOldFont);
	dc.SetBkMode(OPAQUE);	
	dc.SetTextColor(RGB(0,0,0));
}

# endif