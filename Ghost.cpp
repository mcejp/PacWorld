// Ghost.cpp: Implementierung der Klasse CGhost.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "Ghost.h"
#include "helper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int LEFT=1;
static int RIGHT=2;
static int UP=3;
static int DOWN=4;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CGhost::CGhost()
{
	started=false;
	startingPhase=true;	
	newDir=false;
	goWhere=RIGHT;	
	pc1.Start();
	pc2.Start();
	lives=3;
	alive=true;

	dir=Random(2);
}

CGhost::~CGhost()
{

}

void CGhost::MoveGhost()
{		
	//pc1 ist der Taktgeber für die Framerate
	
	UINT sp=10;	

	if(pc1.GetValue()>sp)
	{
		v=1;
		pc1.Reset();
	}
	else v=0;	

	if(int(pc2.GetValue())>(index+1)*4000+1000)
	{
		started=true;
	}

	int locX=x/32+1;
	int locY=y/32+1;

	//ist Paccy in der Nähe?

	if(((paccyX<=locX+5)&&(paccyX>=locX-5))
		&&((paccyY<=locY+5)&&(paccyY>=locY-5)))	
		isNear=true;
	else
		isNear=false;		

	int i;	
	bool onVBorder=false;	//Ghost auf der vert. Grenzlinie?
	bool onHBorder=false;	//Ghost auf der horiz. Grenzlinie?

	int offX=5;
	int offY=5;
	
	bool canMoveUp=false;
	bool canMoveDown=false;
	bool canMoveLeft=false;
	bool canMoveRight=false;

	///Kachel unter Ghost identifizieren///

	int type=Map->GetTile(locX,locY);	

	int above=		Map->GetTile(locX,locY-1);
	int below=		Map->GetTile(locX,locY+1);
	int leftHand=	Map->GetTile(locX-1,locY);
	int rightHand=	Map->GetTile(locX+1,locY);	

	//////32 x 32 Raster///////////

	for(i=0;i<15;i++)
	{
		if(x==(i*32+offX))//x liegt im vert. Raster			
		{
			canMoveDown=true;			
			canMoveUp=true;		
			onVBorder=true;			
		}
	}

	for(i=0;i<13;i++)
	{
		if(y==(i*32+offY))//y liegt im horiz. Raster
		{
			canMoveLeft=true;
			canMoveRight=true;
			onHBorder=true;
		}
	}		

	//Für Ghost: nur Kachelwert bis 5 passierbar

	if(onVBorder&&rightHand>5)
		canMoveRight=false;

	if(onVBorder&&leftHand>5)
		canMoveLeft=false;

	if(onHBorder&&above>5)
		canMoveUp=false;

	if((onHBorder&&below>5)||((x==hideoutX&&y==(hideoutY-32))))			
		canMoveDown=false;	


	if(startingPhase)
		StartGhost();

	else
	{		
	if(newDir)
	{
		newDir=false;		

		if(isNear==false)goWhere=Random(4);	
		
		else
		{
			goWhere=Random(4);	

			bool north=false;
			bool south=false;
			bool west=false;
			bool east=false;	//Richtung von Paccy

			int xDistance=paccyX-locX;
			int yDistance=paccyY-locY;

			if(xDistance<0)	//Paccy ist links vom Ghost
			{
				xDistance=-xDistance;			

				if((paccyY<(locY+2))&&(paccyY>(locY-2)))west=true;
			}
			else if((paccyY<(locY+2))&&(paccyY>(locY-2)))east=true;

			if(yDistance<0)	//Paccy ist oberhalb vom Ghost
			{
				yDistance=-yDistance;

				if((paccyX<(locX+2))&&(paccyX>(locX-2)))north=true;
			}
			else if((paccyX<(locX+2))&&(paccyX>(locX-2)))south=true;			

			if(chased)
			{	
				if(west)				
					canMoveLeft=false;
								
				if(east)				
					canMoveRight=false;
				
				if(north)				
					canMoveUp=false;				

				if(south)				
					canMoveDown=false;				
			}	
			
			else
			{	
				if(west)				
					canMoveRight=false;
								
				if(east)				
					canMoveLeft=false;
				
				if(north)				
					canMoveDown=false;
				
				if(south)				
					canMoveUp=false;				
			}			
		}
	}	

	if(goWhere==0){goDown=true; goUp=false; goLeft=false; goRight=false;}
	else if(goWhere==1){goDown=false; goUp=true; goLeft=false; goRight=false;}
	else if(goWhere==2){goDown=false; goUp=false; goLeft=true; goRight=false;}
	else if(goWhere==3){goDown=false; goUp=false; goLeft=false; goRight=true;}

	//Erst an **Kreuzweg** die neue Richtung einschlagen	

	if((canMoveRight&&canMoveUp)||(canMoveRight&&canMoveDown)
		||(canMoveLeft&&canMoveUp)||(canMoveLeft&&canMoveDown))		
			newDir=true;

	if(goRight&&canMoveRight)
	{right=true;left=false;up=false;down=false;}
	
	if(goLeft&&canMoveLeft)
	{right=false;left=true;up=false;down=false;}
	
	if(goUp&&canMoveUp)
	{right=false;left=false;up=true;down=false;}

	if(goDown&&canMoveDown)
	{right=false;left=false;up=false;down=true;}


	if(alive)
	{
		if(right&&canMoveRight)		{x+=v;	moving=RIGHT;}
		else if(left&&canMoveLeft)	{x-=v;	moving=LEFT;}
		else if(down&&canMoveDown)	{y+=v;	moving=DOWN;}
		else if(up&&canMoveUp)		{y-=v;	moving=UP;}
	}

	int vanish1=-30;
	int vanish2=488;

	if(x<vanish1&&y==leftPassage)	{x=vanish2;	y=rightPassage;}
	if(x>vanish2&&y==rightPassage)	{x=vanish1;	y=leftPassage;}
	}
}

void CGhost::SetMap(CDXMap *map)
{
	Map=map;
}

void CGhost::StartGhost()
{
	int rightBorder=hideoutX+32;
	int leftBorder=hideoutX-32;	

	if(started==false)
	{
		if(dir==1)
			{x-=v; moving=LEFT;}

		else if(dir==0)
			{x+=v; moving=RIGHT;}		

		if(x<=leftBorder)dir=0;
		if(x>=rightBorder)dir=1;
	}
	
	else
	{
		if(x>hideoutX)
			{x-=v; moving=LEFT;}

		else if(x<hideoutX)
			{x+=v; moving=RIGHT;}

		else if(x==hideoutX)
		{
			y-=v; moving=UP;	

			if(y==hideoutY-32)
			{
				goWhere=Random(2)+2;	
				startingPhase=false;			
			}
		}
	}
}

int CGhost::GetX()
{
	return x;
}

int CGhost::GetY()
{
	return y;
}

void CGhost::SetHideout(int hx, int hy)
{
	hideoutX=hx*32+5;
	hideoutY=hy*32+5;

	x=hideoutX-30+index*20;
	y=hideoutY+32;
}

void CGhost::SetPassages(int y1, int y2)
{
	leftPassage=y1*32+5;
	rightPassage=y2*32+5;
}

void CGhost::SetPaccyState(int px, int py, bool ch)
{
	paccyX=px/32+1;
	paccyY=py/32+1;

	chased=ch;
}

void CGhost::Kill()
{
	lives--;	

	if(lives>0)
	{
		startingPhase=true;
		started=false;
		alive=true;
		x=hideoutX-30+index*20;
		y=hideoutY+32;
		pc2.Reset();		
	}
	else
	{
		alive=false;
		x=-100;
		y=-100;
	}
	vulnerable=false;	
}

void CGhost::SetIndex(int i)
{
	index=i;
}

void CGhost::Reset()
{
	alive=true;
	pc2.Reset();
	startingPhase=true;
	started=false;

	x=hideoutX-30+index*20;
	y=hideoutY+32;	

	dir=Random(2);
	vulnerable=false;
}

void CGhost::MoveHome()
{
	onWayHome=true;	

	//pc1 ist der Taktgeber für die Framerate

	if(pc1.GetValue()>6)
	{
		v=1;
		pc1.Reset();
	}
	else v=0;	

	if(x<hideoutX-5)
		{x+=v; moving=RIGHT;}
	else
		if(x>hideoutX+5)
		{x-=v; moving=LEFT;}

	else
		if(y>hideoutY+32)
		{y-=v; moving=UP;}

	else
		if(y<hideoutY+32)
		{y+=v; moving=DOWN;}
	
	else 
	{
		onWayHome=false;
		Kill();
	}
}
