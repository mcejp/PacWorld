// Paccy.cpp: Implementierung der Klasse CPaccy.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "Paccy.h"

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

CPaccy::CPaccy()
{
	pc1.Start();
	moving=RIGHT;	//Anfangsrichtung
	lives=3;
	alive=true;
}

CPaccy::~CPaccy()
{

}

void CPaccy::MovePaccy()
{
	//pc1 ist der Taktgeber für die Framerate

	UINT sp;

	//normal: 10 und 14
	int s1=8;
	int s2=12;

	if(chase||doped)sp=s1;
	else sp=s2;

	if(pc1.GetValue()>sp)
	{
		v=1;
		pc1.Reset();
	}
	else v=0;	

	int i;	
	bool onVBorder=false;	//Paccy auf der vert. Grenzlinie?
	bool onHBorder=false;	//Paccy auf der horiz. Grenzlinie?

	int offX=5;
	int offY=5;
	
	bool canMoveUp=false;
	bool canMoveDown=false;
	bool canMoveLeft=false;
	bool canMoveRight=false;	

	locX=(x+13)/32+1;
	locY=(y+13)/32+1;

	///Kachel unter Paccy identifizieren///

	int type=Map->GetTile(locX,locY);

	tile=type;

	int above=Map->GetTile(locX,locY-1);
	int below=Map->GetTile(locX,locY+1);
	int leftHand=Map->GetTile(locX-1,locY);
	int rightHand=Map->GetTile(locX+1,locY);	

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

	//Kachelwert bis 6 = passierbar

	if(onVBorder&&rightHand>6)
		canMoveRight=false;

	if(onVBorder&&leftHand>6)
		canMoveLeft=false;

	if(onHBorder&&above>6)
		canMoveUp=false;

	if(onHBorder&&below>6)
		canMoveDown=false;	


	if(keyRight&&canMoveRight)
	{right=true;left=false;up=false;down=false;}
	
	if(keyLeft&&canMoveLeft)
	{right=false;left=true;up=false;down=false;}
	
	if(keyUp&&canMoveUp)
	{right=false;left=false;up=true;down=false;}

	if(keyDown&&canMoveDown)
	{right=false;left=false;up=false;down=true;}


	if(right&&canMoveRight)		{x+=v;	moving=RIGHT;}
	else if(left&&canMoveLeft)	{x-=v;	moving=LEFT;}
	else if(down&&canMoveDown)	{y+=v;	moving=DOWN;}
	else if(up&&canMoveUp)		{y-=v;	moving=UP;}	
	else moving=0;

	int vanish1=-30;
	int vanish2=488;

	if(x<vanish1&&y==leftPassage)	{x=vanish2;	y=rightPassage;}
	if(x>vanish2&&y==rightPassage)	{x=vanish1;	y=leftPassage;}
}

void CPaccy::SetMap(CDXMap *map)
{
	Map=map;
}


void CPaccy::SetStartPos(int x, int y)
{
	this->x=x*32+5;
	this->y=y*32+5;
}

void CPaccy::SetPassages(int y1, int y2)
{
	leftPassage=y1*32+5;
	rightPassage=y2*32+5;
}

void CPaccy::Kill()
{
	if(lives<1)
		alive=false;

	right=left=up=down=false;
	keyUp=keyDown=keyLeft=keyRight=false;
}
