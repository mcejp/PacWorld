// GameWindow.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "resource.h"
#include "GameWindow.h"
#include "GameApp.h"
#include "AboutBox.h"
#include "helper.h"
#include "Tutorial.h"
#include "Settings.h"
#include "Highscore.h"
#include "Thanks.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int LEFT=1;
static int RIGHT=2;
static int UP=3;
static int DOWN=4;
/////////////////////////////////////////////////////////////////////////////
// GameWindow

static int windowW=531;
static int windowH=525;	//Width and Height of the Window


GameWindow::GameWindow(LPCSTR ClassName)
{	 
	//Center the Window on the Screen
    int xoffset=(::GetSystemMetrics(SM_CXSCREEN) - windowW) / 2; 
	int yoffset=(::GetSystemMetrics(SM_CYSCREEN) - windowH) / 2;

	Create(ClassName,"PacWorld",
		WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,
		CRect(xoffset,yoffset,xoffset+windowW,yoffset+windowH)
		,NULL,"MAINMENU");		

	CMenu* pSystemMenu = GetSystemMenu (FALSE);
	pSystemMenu->DeleteMenu(SC_SIZE, MF_BYCOMMAND);
	pSystemMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
	
	InitDirectDraw();

	pc1.Start();
	pc2.Start();	
	pc3.Start();
	pc4.Start();
	pc5.Start();
	pc6.Start();
	pc7.Start();

	SetTimer(1,100,NULL);	

	LoadSettings();

	fullVersion=true;	

	level=1;

	intro=true;

	NewGame();

	paccyAlive=true;	

	GameApp* gApp = (GameApp*)AfxGetApp();
    gApp->gw = this;
}

BOOL GameWindow::InitDirectDraw()
{	
	Screen = new CDXScreen();

	GetClientRect(&rect);

	Screen->CreateWindowed(m_hWnd,rect.right, rect.bottom);

	Screen->GetBack()->ChangeFont("Comic Sans MS", 0, 15);
	
	// Load the WAD file and open it
	
	Resource = new CDXResource();
	Resource->Open("data.wad");
	
	char *forestR=	Resource->GetFile("forest.bmp");	
	ForestT =	new CDXTile(Screen, forestR, 32, 32, 40);

	char *iceR=		Resource->GetFile("ice.bmp");	
	IceT =		new CDXTile(Screen, iceR, 32, 32, 40);

	char *cityR=	Resource->GetFile("city.bmp");	
	CityT =		new CDXTile(Screen, cityR, 32, 32, 40);

	char *desertR=	Resource->GetFile("desert.bmp");	
	DesertT =	new CDXTile(Screen, desertR, 32, 32, 40);

	char *nightR=	Resource->GetFile("night.bmp");	
	NightT =	new CDXTile(Screen, nightR, 32, 32, 40);

	char *fireR=	Resource->GetFile("fire.bmp");	
	FireT =		new CDXTile(Screen, fireR, 32, 32, 40);

	char *introTilesR=	Resource->GetFile("introtiles.bmp");	
	IntroT =	new CDXTile(Screen, introTilesR, 32, 32, 3);

	
	// Create and load the map
	Map = new CDXMap(IntroT, Screen);
	
	Map->MoveTo(12,-8);

	char *paccyR=	Resource->GetFile("paccy.bmp");	

	Paccy = new CDXSprite(Screen, paccyR, 25, 25, 30);	

	Paccy->ColorKey(0);		
	
	char *introR=	Resource->GetFile("intro.bmp");	
	Intro = new CDXSprite(Screen, introR, 235, 46, 2);	

	Intro->ColorKey(0);		

	char *titlesR=	Resource->GetFile("titles.bmp");
	Titles = new CDXSprite(Screen, titlesR, 150, 50, 6);	

	Titles->ColorKey(0);		

	char *gadgetsR=	Resource->GetFile("gadgets.bmp");	
	Gadget = new CDXSprite(Screen, gadgetsR, 27, 13, 12);	

	Gadget->ColorKey(0);			

	char *ghostsR=	Resource->GetFile("ghosts.bmp");		
	for(i=0;i<5;i++)
	{
		Ghost[i] = new CDXSprite(Screen, ghostsR, 25, 26, 48);	
		Ghost[i]->ColorKey(0);	
	}	

	Sound = new CDXSound;
	Sound->Create(m_hWnd);

	char *chompR=	Resource->GetFile("chomp.wav");
	ChompSound = new CDXSoundBuffer;
	ChompSound->Load(Sound, chompR);

	char *catchR=	Resource->GetFile("catch.wav");
	CatchSound = new CDXSoundBuffer;
	CatchSound->Load(Sound, catchR);

	char *foundR=	Resource->GetFile("found.wav");
	FoundSound = new CDXSoundBuffer;
	FoundSound->Load(Sound, foundR);

	char *itemR=	Resource->GetFile("item.wav");
	ItemSound = new CDXSoundBuffer;
	ItemSound->Load(Sound, itemR);

	char *killedR=	Resource->GetFile("killed.wav");
	KilledSound = new CDXSoundBuffer;
	KilledSound->Load(Sound, killedR);

	char *triumphR=	Resource->GetFile("triumph.wav");
	TriumphSound = new CDXSoundBuffer;
	TriumphSound->Load(Sound, triumphR);	

	Resource->Close();

	return TRUE;
}

GameWindow::~GameWindow()
{	
	SaveSettings();

	Screen->GetBack()->ReleaseDC();

	delete ForestT;
	delete IceT;
	delete CityT;
	delete DesertT;
	delete NightT;
	delete FireT;
	delete IntroT;
	delete Map;	
	delete Paccy;
	delete Intro;
	delete Titles;
	delete Gadget;
	delete Ghost[0];
	delete Ghost[1];
	delete Ghost[2];
	delete Ghost[3];
	delete Screen;	
}


BEGIN_MESSAGE_MAP(GameWindow, CFrameWnd)
	//{{AFX_MSG_MAP(GameWindow)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()	
	ON_WM_KEYUP()
	ON_WM_TIMER()
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_HELP_HOWTOPLAY, OnTutorial)
	ON_COMMAND(ID_GAME_NEW, OnNewGame)
	ON_COMMAND(ID_GAME_PAUSE, OnPause)
	ON_COMMAND(ID_GAME_EXIT, OnGameExit)
	ON_COMMAND(ID_OPTIONS_SETTINGS, OnSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten GameWindow 

void GameWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	firstKeyPress=true;

	if(nChar==VK_ESCAPE&&intro==true)
	{
		intro=false;
		Map->MoveTo(12,-8);
		NewGame();
	}	

	if(paccyAlive&&intro==false)
	{
		if(nChar=='P'||nChar=='p')paused^=true;
		else paused=false;

		//Häkchen vor Pausemenü setzen
		CMenu* pMenu = GetMenu()->GetSubMenu(0);

		if(paused)
			pMenu->CheckMenuItem( ID_GAME_PAUSE, MF_CHECKED );
		else
			pMenu->CheckMenuItem( ID_GAME_PAUSE, MF_UNCHECKED );

		switch (nChar)
		{
			case VK_UP:		pac.keyUp=true;		break;

			case VK_DOWN:	pac.keyDown=true;	break;	

			case VK_LEFT:	pac.keyLeft=true;	break;	

			case VK_RIGHT:	pac.keyRight=true;	break;			

			case VK_F12:	int maxLevel;
							if(fullVersion)maxLevel=12;
							else maxLevel=4;
							if(level<maxLevel)level++;
							else level=1;

							NewGame();				break;				
		}
	}
}

void GameWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{		
	switch (nChar)
	{
		case VK_UP:		pac.keyUp=false;		break;

		case VK_DOWN:	pac.keyDown=false;		break;	

		case VK_LEFT:	pac.keyLeft=false;		break;	

		case VK_RIGHT:	pac.keyRight=false;		break;	
	}		
}

void GameWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{		
	CFrameWnd::OnLButtonDown(nFlags, point);	
}

void GameWindow::OnRButtonDown(UINT nFlags, CPoint point) 
{	
	CFrameWnd::OnRButtonDown(nFlags, point);		
}

void GameWindow::OnTimer(UINT nIDEvent) 
{	
	if(intro)
		PlayIntro();
	else
		DrawStuff();

	CFrameWnd::OnTimer(nIDEvent);
}

void GameWindow::OnHelpAbout() 
{	
	AboutBox about;	
		
	about.DoModal();		
}

void GameWindow::OnTutorial()
{
	CTutorial tut;

	tut.DoModal();
}

void GameWindow::OnGameExit() 
{
	DestroyWindow();	
}

void GameWindow::MoveGame() 
{
	if(intro)
		PlayIntro();
	else if(!paused)
	{
		pac.MovePaccy();		

		EatCookies();

		PlaceGadgets();
		
		SetAnimationFrames();		

		if(pills>0)
			Paccy->SetPos(pac.x+20, pac.y+40);	
		else
		{
			pac.left=false;	pac.right=false;
			pac.up=false;	pac.down=false;
		}

		for(i=0;i<4;i++)
		{				
			if(pills>0&&ghost[i].onWayHome==false)		
				ghost[i].MoveGhost();

			else if(pills>0&&ghost[i].onWayHome)
				ghost[i].MoveHome();

			ghost[i].SetPaccyState(pac.x, pac.y, pac.chase);
			Ghost[i]->SetPos(ghost[i].GetX()+20, ghost[i].GetY()+40);		

			if(Ghost[i]->SpriteHit(Paccy)&&paccyAlive&&pills>0)
			{
				if(pac.chase&&ghost[i].vulnerable)
				{						
					if(ghost[i].onWayHome==false)
					{
						if(sound>0)KilledSound->Play();
						ghost[i].MoveHome();
						
						int killedGhostValue=pac.lives*25;
						points+=killedGhostValue;
						
						TriggerPopUpPoints(killedGhostValue);
					}
				}

				else
				{					
					if(sound>0)CatchSound->Play();
					pac.lives--;
					pac.Kill();				

					paccyAlive=false;					
				}
			}
		}

		DrawStuff();
	}
}

void GameWindow::DrawStuff()
{	
	Map->DrawClipped(Screen->GetBack(),&rect);	

	DrawGadgets();
	
	Paccy->DrawClipped(Screen->GetBack(), &rect);

	for(i=0;i<4;i++)		
		Ghost[i]->DrawClipped(Screen->GetBack(), &rect);	

	DrawPoints(popPoints);

	ShowTitle();
	
	Screen->Flip();  // Then display it		
}

void GameWindow::DrawDebugText(int x, int y, CString s)
{
	Screen->GetBack()->GetDC();	
	Screen->GetBack()->SetFont();
	Screen->GetBack()->TextXY(x+1,y+1,RGB(0,0,0),s);
	Screen->GetBack()->TextXY(x,y,RGB(255,255,255),s);
	Screen->GetBack()->ReleaseDC();
}

void GameWindow::SetAnimationFrames()
{
	if(pc1.GetValue()>450)	pc1.Reset();

	static int lastPaccyDir;
	static bool movedLeft=false;	

	if(pac.moving!=0&&firstKeyPress)
		lastPaccyDir=pac.moving;	

	if(lastPaccyDir==LEFT)movedLeft=true;
	if(lastPaccyDir==RIGHT)movedLeft=false;		

	if(pills==0&&fired3==true)	//Level geschafft 
	{
		if(pc4.GetValue()>1000)
		{
			Paccy->SetFrame(29);
		}

		else if(pc4.GetValue()>800)
		{
			Paccy->SetFrame(28);
		}

		else if(pc4.GetValue()>600)
		{
			Paccy->SetFrame(27);
		}

		else if(pc4.GetValue()>400)
		{
			Paccy->SetFrame(26);
		}

		else if(pc4.GetValue()>200)
		{
			Paccy->SetFrame(25);
		}

		else if(pc4.GetValue()<=200)
		{
			if(pc4.GetValue()>190)
				if(sound>0)TriumphSound->Play();

			Paccy->SetFrame(24);
		}		

		if(pc4.GetValue()>2500)
		{
			fired3=false;			

			pc1.Reset();

			pac.Kill();

			//Höchsten Level hier eintragen
			int maxLevel;

			if(fullVersion)
				maxLevel=12;
			else
				maxLevel=4;

			if(level<maxLevel)
				level++;
			else
			{
				CHighscore hd;
				hd.highscore=highscore;
				hd.score=points;

				if(hd.DoModal()==IDOK)
					highscore=hd.highscore;

				level=1;
			}

			NewGame();				
		}
	}

	else if(paccyAlive==false)	//Sterbesequenz abspielen
	{
		static bool fired2=false;

		if(fired2==false)	
		{
			pc3.Reset();
			fired2=true;
		}		

		if(pc3.GetValue()>1500)
		{
			Paccy->SetFrame(23);
		}

		else if(pc3.GetValue()>1200)
		{
			Paccy->SetFrame(22);
		}

		else if(pc3.GetValue()>900)
		{
			Paccy->SetFrame(21);
		}

		else if(pc3.GetValue()>600)
		{
			Paccy->SetFrame(20);
		}

		else if(pc3.GetValue()>300)
		{
			Paccy->SetFrame(19);
		}

		else
		{
			Paccy->SetFrame(18);
		}		

		if(pc3.GetValue()>2000)
		{
			fired2=false;
			
			pc1.Reset();	
			
			if(pac.lives==0)
			{
				level=1;	//Zurück zum ersten Level!

				CHighscore hd;
				hd.highscore=highscore;
				hd.score=points;

				if(hd.DoModal()==IDOK)
					highscore=hd.highscore;

				NewGame();
			}
			else 
				RestartLevel();
		}
	}

	else if(pac.moving)
	{
		///Animationsphasen für Paccy

		if(pc1.GetValue()>300)			
		{
			if(lastPaccyDir==LEFT)		Paccy->SetFrame(2);
			else if(lastPaccyDir==RIGHT)	Paccy->SetFrame(5);
			else if(lastPaccyDir==UP)
				if(movedLeft)Paccy->SetFrame(14);
						else Paccy->SetFrame(8);
			else if(lastPaccyDir==DOWN)
				if(movedLeft)Paccy->SetFrame(17);
						else Paccy->SetFrame(11);				 
		}
		else if(pc1.GetValue()>150)			
		{				
				 if(lastPaccyDir==LEFT)		Paccy->SetFrame(1);
			else if(lastPaccyDir==RIGHT)	Paccy->SetFrame(4);
			else if(lastPaccyDir==UP)
				if(movedLeft)Paccy->SetFrame(13);
						else Paccy->SetFrame(7);
			else if(lastPaccyDir==DOWN)
				if(movedLeft)Paccy->SetFrame(16);
						else Paccy->SetFrame(10);
		}
		else 
		{	
				 if(lastPaccyDir==LEFT)		Paccy->SetFrame(0);
			else if(lastPaccyDir==RIGHT)	Paccy->SetFrame(3);
			else if(lastPaccyDir==UP)
				if(movedLeft)Paccy->SetFrame(12);
						else Paccy->SetFrame(6);
			else if(lastPaccyDir==DOWN)
				if(movedLeft)Paccy->SetFrame(15);
						else Paccy->SetFrame(9);
		}
	}	
		
	///Animationsphasen für die Ghosts

	bool scared1=false;
	bool scared2=false;
	bool scared3=false;
	bool scared4=false;

	if(pac.chase&&ghost[0].vulnerable)	scared1=true;
	if(pac.chase&&ghost[1].vulnerable)	scared2=true;
	if(pac.chase&&ghost[2].vulnerable)	scared3=true;
	if(pac.chase&&ghost[3].vulnerable)	scared4=true;

	if(pc1.GetValue()>225)
	{
		if(ghost[0].moving==LEFT)		Ghost[0]->SetFrame(scared1?32:0);
		else if(ghost[0].moving==RIGHT) Ghost[0]->SetFrame(scared1?34:2);
		else if(ghost[0].moving==UP)	Ghost[0]->SetFrame(scared1?36:4);
		else if(ghost[0].moving==DOWN)	Ghost[0]->SetFrame(scared1?38:6);

			 if(ghost[1].moving==LEFT)	Ghost[1]->SetFrame(scared2?32:8);
		else if(ghost[1].moving==RIGHT) Ghost[1]->SetFrame(scared2?34:10);
		else if(ghost[1].moving==UP)	Ghost[1]->SetFrame(scared2?36:12);
		else if(ghost[1].moving==DOWN)	Ghost[1]->SetFrame(scared2?38:14);

			 if(ghost[2].moving==LEFT)	Ghost[2]->SetFrame(scared3?32:16);
		else if(ghost[2].moving==RIGHT) Ghost[2]->SetFrame(scared3?34:18);
		else if(ghost[2].moving==UP)	Ghost[2]->SetFrame(scared3?36:20);
		else if(ghost[2].moving==DOWN)	Ghost[2]->SetFrame(scared3?38:22);

			 if(ghost[3].moving==LEFT)	Ghost[3]->SetFrame(scared4?32:24);
		else if(ghost[3].moving==RIGHT) Ghost[3]->SetFrame(scared4?34:26);
		else if(ghost[3].moving==UP)	Ghost[3]->SetFrame(scared4?36:28);
		else if(ghost[3].moving==DOWN)	Ghost[3]->SetFrame(scared4?38:30);
	
	}
	else
	{
			 if(ghost[0].moving==LEFT)	Ghost[0]->SetFrame(scared1?33:1);
		else if(ghost[0].moving==RIGHT) Ghost[0]->SetFrame(scared1?35:3);
		else if(ghost[0].moving==UP)	Ghost[0]->SetFrame(scared1?37:5);
		else if(ghost[0].moving==DOWN)	Ghost[0]->SetFrame(scared1?39:7);

			 if(ghost[1].moving==LEFT)	Ghost[1]->SetFrame(scared2?33:9);
		else if(ghost[1].moving==RIGHT) Ghost[1]->SetFrame(scared2?35:11);
		else if(ghost[1].moving==UP)	Ghost[1]->SetFrame(scared2?37:13);
		else if(ghost[1].moving==DOWN)	Ghost[1]->SetFrame(scared2?39:15);

			 if(ghost[2].moving==LEFT)	Ghost[2]->SetFrame(scared3?33:17);
		else if(ghost[2].moving==RIGHT) Ghost[2]->SetFrame(scared3?35:19);
		else if(ghost[2].moving==UP)	Ghost[2]->SetFrame(scared3?37:21);
		else if(ghost[2].moving==DOWN)	Ghost[2]->SetFrame(scared3?39:23);

			 if(ghost[3].moving==LEFT)	Ghost[3]->SetFrame(scared4?33:25);
		else if(ghost[3].moving==RIGHT) Ghost[3]->SetFrame(scared4?35:27);
		else if(ghost[3].moving==UP)	Ghost[3]->SetFrame(scared4?37:29);
		else if(ghost[3].moving==DOWN)	Ghost[3]->SetFrame(scared4?39:31);
	}

	for(i=0;i<4;i++)
	{
		if(ghost[i].onWayHome)
		{
			if(ghost[i].moving==LEFT)	Ghost[i]->SetFrame(40);
			if(ghost[i].moving==RIGHT)	Ghost[i]->SetFrame(41);
			if(ghost[i].moving==UP)		Ghost[i]->SetFrame(42);
			if(ghost[i].moving==DOWN)	Ghost[i]->SetFrame(43);
		}
	}

	///Animationsphasen für die Kacheln

	//Tür öffnen, wenn Paccy Schlüssel hat

	if(pac.hasKey)
	{
		if(diamondCollected==false)
			diamondVisible=true;

		for(i=0; i<17; i++)
			for(j=0; j<15; j++)
				if(Map->GetTile(i, j)==7)
					Map->SetTile(i, j, 6);
	}

	//Grosse "Kekse" animieren - 4 Phasen

	if(pc2.GetValue()>1000)	pc2.Reset();

	if(pc2.GetValue()>750)
	{
		for(i=0; i<17; i++)
		for(j=0; j<15; j++)
			if((Map->GetTile(i, j)>2)&&(Map->GetTile(i, j)<6))
				Map->SetTile(i, j, 3);
	}

	else if(pc2.GetValue()>500)
	{
		for(i=0; i<17; i++)
		for(j=0; j<15; j++)
			if((Map->GetTile(i, j)>2)&&(Map->GetTile(i, j)<6))
				Map->SetTile(i, j, 4);
	}

	else if(pc2.GetValue()>250)
	{
		for(i=0; i<17; i++)
		for(j=0; j<15; j++)
			if((Map->GetTile(i, j)>2)&&(Map->GetTile(i, j)<6))
				Map->SetTile(i, j, 5);
	}

	else 
	{
		for(i=0; i<17; i++)
		for(j=0; j<15; j++)
			if((Map->GetTile(i, j)>2)&&(Map->GetTile(i, j)<6))
				Map->SetTile(i, j, 4);
	}

	///Zeitsteuerung für Paccy's "chase"-Zustand	
		
	bool skipBlink;
	static bool ok1=false;	
	UINT w1=9200;

	if(pc3.GetValue()<=w1-1600)
		//zweiter Keks gefressen
			skipBlink=true;
		else 
			skipBlink=false;

	if(pillEaten)	
	{
		ok1=true;
		pillEaten=false;
		pc3.Reset();
		pac.chase=true;
	}
	
	if(skipBlink==false&&ok1)
	{
		if(pc3.GetValue()>w1)
		
			pac.chase=false;					

		else if(pc3.GetValue()>w1-400)	pac.chase=true;		

		else if(pc3.GetValue()>w1-800)	pac.chase=false;		

		else if(pc3.GetValue()>w1-1200)	pac.chase=true;		

		else if(pc3.GetValue()>w1-1600)	pac.chase=false;		
	
	}
	else
	{
		if(pc3.GetValue()>w1)
		{
			pac.chase=false;			
		}
	}	
}

void GameWindow::EatCookies()
{
	int x=(pac.x+13)/32+1;
	int y=(pac.y+13)/32+1;

	//Kleine Pille
	if(pac.tile==2)
	{	
		points+=pac.lives;
		pills--;

		if(pac.moving!=0&&sound>1)ChompSound->Play();

		Map->SetTile(x, y, 1);		
	}
	//Grosse Pille
	else if(pac.tile==3||pac.tile==4||pac.tile==5)
	{
		if(pills>0)
		{
			if(sound>0)
				FoundSound->Play();		

			Map->SetTile(x, y, 1);

			pillEaten=true;	
		}
		
		for(i=0;i<4;i++)
			if(ghost[i].startingPhase==false)
				ghost[i].vulnerable=true;
	}

	//Kleine Pillen zählen
	pills=0;

	for(i=0; i<17; i++)
		for(j=0; j<15; j++)
			if(Map->GetTile(i, j)==2)
				pills++;	

	if(fired3==false&&pills==0)	
	{									
		fired3=true;
		pc4.Reset();
	}
	
	//Gadgets aufsammeln

	if(pac.locX-1==gLoc[level-1].x[0]
		&&pac.locY-1==gLoc[level-1].y[0]
		&&keyVisible==true)
	{
		if(sound>0)ItemSound->Play();

		keyVisible=false;		//Schlüssel
		pac.hasKey=true;

		pc5.Reset();
	}

	if(pac.locX-1==gLoc[level-1].x[5]
		&&pac.locY-1==gLoc[level-1].y[5]
		&&diamondVisible==true)
	{
		if(sound>0)ItemSound->Play();

		int diamondValue;
		if(pac.lives==1)diamondValue=50;
		if(pac.lives==2)diamondValue=100;
		if(pac.lives==3)diamondValue=200;

		points+=diamondValue;
		TriggerPopUpPoints(diamondValue);

		diamondVisible=false;		//Diamant
		diamondCollected=true;
		diamonds++;		
	}

	if(pac.locX-1==curGadgetX&&pac.locY-1==curGadgetY
		&&gadgetActive==true)
	{
		if(sound>0)ItemSound->Play();

		int fruitValue=pac.lives*15;
		points+=fruitValue;
		TriggerPopUpPoints(fruitValue);

		gadgetActive=false;		//Frucht
			
		pac.doped=true;	

		pc5.Reset();
	}
}

void GameWindow::NewGame()
{	
	pc6.Reset();

	pac.Kill();	

	fired3=false;

	Paccy->SetFrame(24);
	
	pac.hasKey=false;

	pac.lives=3;

	pac.alive=true;
	
	pac.chase=false;	
	
	pac.doped=false;
	
	if(fullVersion)
		SetLevelInfo();
	else
		Set4LevelInfo();

	points-=3;
					
	for(i=0;i<4;i++)
	{
		ghost[i].Reset();
		ghost[i].lives=3;
	}

	paccyAlive=true;

	keyVisible=false;

	diamondVisible=false;

	diamondCollected=false;

	gadgetActive=false;

	givenGadgets=0;

	pc1.Reset();
	pc2.Reset();
	pc3.Reset();
	pc5.Reset();
}

//Die vier Levels der Shareware-Version

void GameWindow::Set4LevelInfo()
{
	if(level==1)
	{
		points=0;

		Resource->Open("data.wad");			
		char *forMap1R=	Resource->GetFile("forest1.map");	
		Map->Load(forMap1R);
		Resource->Close();

		if(intro==false)
			Map->LoadTiles(ForestT);

		gLoc[0].x[0]=2;
		gLoc[0].y[0]=0;

		gLoc[0].x[1]=6;
		gLoc[0].y[1]=2;

		gLoc[0].x[2]=14;
		gLoc[0].y[2]=3;

		gLoc[0].x[3]=3;
		gLoc[0].y[3]=11;

		gLoc[0].x[4]=12;
		gLoc[0].y[4]=9;

		gLoc[0].x[5]=12;
		gLoc[0].y[5]=3;

		pac.SetMap(Map);
		pac.SetStartPos(9, 9);
		pac.SetPassages(4, 9);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(7,3);
			ghost[i].SetPassages(4, 9);	
		}
	}

	if(level==2)
	{
		Resource->Open("data.wad");			
		char *iceMap2R=	Resource->GetFile("ice2.map");	
		Map->Load(iceMap2R);
		Resource->Close();

		Map->LoadTiles(IceT);

		gLoc[1].x[0]=8;
		gLoc[1].y[0]=2;

		gLoc[1].x[1]=2;
		gLoc[1].y[1]=4;

		gLoc[1].x[2]=11;
		gLoc[1].y[2]=2;

		gLoc[1].x[3]=2;
		gLoc[1].y[3]=8;

		gLoc[1].x[4]=12;
		gLoc[1].y[4]=8;

		gLoc[1].x[5]=3;
		gLoc[1].y[5]=2;

		pac.SetMap(Map);
		pac.SetStartPos(6, 5);
		pac.SetPassages(4, 6);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(11,3);
			ghost[i].SetPassages(4, 6);	
		}
	}

	if(level==3)
	{
		Resource->Open("data.wad");			
		char *cityMap2R=	Resource->GetFile("city2.map");	
		Map->Load(cityMap2R);
		Resource->Close();

		Map->LoadTiles(CityT);

		gLoc[2].x[0]=11;
		gLoc[2].y[0]=9;

		gLoc[2].x[1]=4;
		gLoc[2].y[1]=1;

		gLoc[2].x[2]=11;
		gLoc[2].y[2]=1;

		gLoc[2].x[3]=2;
		gLoc[2].y[3]=8;

		gLoc[2].x[4]=9;
		gLoc[2].y[4]=12;

		gLoc[2].x[5]=7;
		gLoc[2].y[5]=2;

		pac.SetMap(Map);
		pac.SetStartPos(6, 4);
		pac.SetPassages(8, 5);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(11,6);
			ghost[i].SetPassages(8, 5);	
		}
	}

	if(level==4)
	{
		Resource->Open("data.wad");			
		char *nightMap1R=	Resource->GetFile("night1.map");	
		Map->Load(nightMap1R);
		Resource->Close();

		Map->LoadTiles(NightT);

		gLoc[3].x[0]=10;
		gLoc[3].y[0]=2;

		gLoc[3].x[1]=2;
		gLoc[3].y[1]=3;

		gLoc[3].x[2]=13;
		gLoc[3].y[2]=5;

		gLoc[3].x[3]=5;
		gLoc[3].y[3]=10;

		gLoc[3].x[4]=12;
		gLoc[3].y[4]=9;

		gLoc[3].x[5]=3;
		gLoc[3].y[5]=10;

		pac.SetMap(Map);
		pac.SetStartPos(6, 3);
		pac.SetPassages(7, 5);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(11, 6);
			ghost[i].SetPassages(7, 5);	
		}
	}
}

void GameWindow::SetLevelInfo()
{
	if(level==1)
	{
		points=0;

		Resource->Open("data.wad");			
		char *forMap1R=	Resource->GetFile("forest1.map");	
		Map->Load(forMap1R);
		Resource->Close();

		if(intro==false)
			Map->LoadTiles(ForestT);

		gLoc[0].x[0]=2;
		gLoc[0].y[0]=0;

		gLoc[0].x[1]=6;
		gLoc[0].y[1]=2;

		gLoc[0].x[2]=14;
		gLoc[0].y[2]=3;

		gLoc[0].x[3]=3;
		gLoc[0].y[3]=11;

		gLoc[0].x[4]=12;
		gLoc[0].y[4]=9;

		gLoc[0].x[5]=12;
		gLoc[0].y[5]=3;

		pac.SetMap(Map);
		pac.SetStartPos(9, 9);
		pac.SetPassages(4, 9);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(7,3);
			ghost[i].SetPassages(4, 9);	
		}
	}

	if(level==2)
	{
		Resource->Open("data.wad");			
		char *forMap2R=	Resource->GetFile("forest2.map");	
		Map->Load(forMap2R);
		Resource->Close();

		Map->LoadTiles(ForestT);

		gLoc[1].x[0]=4;
		gLoc[1].y[0]=5;

		gLoc[1].x[1]=4;
		gLoc[1].y[1]=1;

		gLoc[1].x[2]=14;
		gLoc[1].y[2]=2;

		gLoc[1].x[3]=1;
		gLoc[1].y[3]=7;

		gLoc[1].x[4]=9;
		gLoc[1].y[4]=11;

		gLoc[1].x[5]=11;
		gLoc[1].y[5]=8;

		pac.SetMap(Map);
		pac.SetStartPos(8, 6);
		pac.SetPassages(5, 6);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(3,9);
			ghost[i].SetPassages(5, 6);	
		}
	}

	if(level==3)
	{
		Resource->Open("data.wad");			
		char *iceMap1R=	Resource->GetFile("ice1.map");	
		Map->Load(iceMap1R);
		Resource->Close();

		Map->LoadTiles(IceT);

		gLoc[2].x[0]=11;
		gLoc[2].y[0]=4;

		gLoc[2].x[1]=0;
		gLoc[2].y[1]=3;

		gLoc[2].x[2]=9;
		gLoc[2].y[2]=3;

		gLoc[2].x[3]=3;
		gLoc[2].y[3]=9;

		gLoc[2].x[4]=14;
		gLoc[2].y[4]=9;

		gLoc[2].x[5]=2;
		gLoc[2].y[5]=3;

		pac.SetMap(Map);
		pac.SetStartPos(8, 8);
		pac.SetPassages(8, 4);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(11,9);
			ghost[i].SetPassages(8, 4);	
		}
	}

	if(level==4)
	{
		Resource->Open("data.wad");			
		char *iceMap2R=	Resource->GetFile("ice2.map");	
		Map->Load(iceMap2R);
		Resource->Close();

		Map->LoadTiles(IceT);

		gLoc[3].x[0]=8;
		gLoc[3].y[0]=2;

		gLoc[3].x[1]=2;
		gLoc[3].y[1]=4;

		gLoc[3].x[2]=11;
		gLoc[3].y[2]=2;

		gLoc[3].x[3]=2;
		gLoc[3].y[3]=8;

		gLoc[3].x[4]=12;
		gLoc[3].y[4]=8;

		gLoc[3].x[5]=3;
		gLoc[3].y[5]=2;

		pac.SetMap(Map);
		pac.SetStartPos(6, 5);
		pac.SetPassages(4, 6);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(11,3);
			ghost[i].SetPassages(4, 6);	
		}
	}

	if(level==5)
	{
		Resource->Open("data.wad");			
		char *cityMap1R=	Resource->GetFile("city1.map");	
		Map->Load(cityMap1R);
		Resource->Close();

		Map->LoadTiles(CityT);

		gLoc[4].x[0]=10;
		gLoc[4].y[0]=4;

		gLoc[4].x[1]=2;
		gLoc[4].y[1]=3;

		gLoc[4].x[2]=12;
		gLoc[4].y[2]=0;

		gLoc[4].x[3]=2;
		gLoc[4].y[3]=9;

		gLoc[4].x[4]=12;
		gLoc[4].y[4]=11;

		gLoc[4].x[5]=9;
		gLoc[4].y[5]=10;

		pac.SetMap(Map);
		pac.SetStartPos(8, 7);
		pac.SetPassages(9, 5);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(5,4);
			ghost[i].SetPassages(9, 5);	
		}
	}

	if(level==6)
	{
		Resource->Open("data.wad");			
		char *cityMap2R=	Resource->GetFile("city2.map");	
		Map->Load(cityMap2R);
		Resource->Close();

		Map->LoadTiles(CityT);

		gLoc[5].x[0]=11;
		gLoc[5].y[0]=9;

		gLoc[5].x[1]=4;
		gLoc[5].y[1]=1;

		gLoc[5].x[2]=11;
		gLoc[5].y[2]=1;

		gLoc[5].x[3]=2;
		gLoc[5].y[3]=8;

		gLoc[5].x[4]=9;
		gLoc[5].y[4]=12;

		gLoc[5].x[5]=7;
		gLoc[5].y[5]=2;

		pac.SetMap(Map);
		pac.SetStartPos(6, 4);
		pac.SetPassages(8, 5);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(11,6);
			ghost[i].SetPassages(8, 5);	
		}
	}

	if(level==7)
	{
		Resource->Open("data.wad");			
		char *desertMap1R=	Resource->GetFile("desert1.map");	
		Map->Load(desertMap1R);
		Resource->Close();

		Map->LoadTiles(DesertT);

		gLoc[6].x[0]=2;
		gLoc[6].y[0]=5;

		gLoc[6].x[1]=4;
		gLoc[6].y[1]=2;

		gLoc[6].x[2]=13;
		gLoc[6].y[2]=2;

		gLoc[6].x[3]=2;
		gLoc[6].y[3]=10;

		gLoc[6].x[4]=10;
		gLoc[6].y[4]=11;

		gLoc[6].x[5]=12;
		gLoc[6].y[5]=10;

		pac.SetMap(Map);
		pac.SetStartPos(7, 7);
		pac.SetPassages(5, 1);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(7, 1);
			ghost[i].SetPassages(5, 1);	
		}
	}

	if(level==8)
	{
		Resource->Open("data.wad");			
		char *desertMap2R=	Resource->GetFile("desert2.map");	
		Map->Load(desertMap2R);
		Resource->Close();

		Map->LoadTiles(DesertT);

		gLoc[7].x[0]=4;
		gLoc[7].y[0]=8;

		gLoc[7].x[1]=4;
		gLoc[7].y[1]=3;

		gLoc[7].x[2]=11;
		gLoc[7].y[2]=5;

		gLoc[7].x[3]=6;
		gLoc[7].y[3]=11;

		gLoc[7].x[4]=13;
		gLoc[7].y[4]=10;

		gLoc[7].x[5]=11;
		gLoc[7].y[5]=2;

		pac.SetMap(Map);
		pac.SetStartPos(8, 7);
		pac.SetPassages(8, 3);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(3, 9);
			ghost[i].SetPassages(8, 3);	
		}
	}

	if(level==9)
	{
		Resource->Open("data.wad");			
		char *nightMap1R=	Resource->GetFile("night1.map");	
		Map->Load(nightMap1R);
		Resource->Close();

		Map->LoadTiles(NightT);

		gLoc[8].x[0]=10;
		gLoc[8].y[0]=2;

		gLoc[8].x[1]=2;
		gLoc[8].y[1]=3;

		gLoc[8].x[2]=13;
		gLoc[8].y[2]=5;

		gLoc[8].x[3]=5;
		gLoc[8].y[3]=10;

		gLoc[8].x[4]=12;
		gLoc[8].y[4]=9;

		gLoc[8].x[5]=3;
		gLoc[8].y[5]=10;

		pac.SetMap(Map);
		pac.SetStartPos(6, 3);
		pac.SetPassages(7, 5);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(11, 6);
			ghost[i].SetPassages(7, 5);	
		}
	}

	if(level==10)
	{
		Resource->Open("data.wad");			
		char *nightMap2R=	Resource->GetFile("night2.map");	
		Map->Load(nightMap2R);
		Resource->Close();

		Map->LoadTiles(NightT);

		gLoc[9].x[0]=3;
		gLoc[9].y[0]=8;

		gLoc[9].x[1]=3;
		gLoc[9].y[1]=0;

		gLoc[9].x[2]=9;
		gLoc[9].y[2]=2;

		gLoc[9].x[3]=4;
		gLoc[9].y[3]=12;

		gLoc[9].x[4]=10;
		gLoc[9].y[4]=8;

		gLoc[9].x[5]=12;
		gLoc[9].y[5]=2;

		pac.SetMap(Map);
		pac.SetStartPos(4, 6);
		pac.SetPassages(6, 5);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(5, 9);
			ghost[i].SetPassages(6, 5);	
		}
	}

	if(level==11)
	{
		Resource->Open("data.wad");			
		char *fireMap1R=	Resource->GetFile("fire1.map");	
		Map->Load(fireMap1R);
		Resource->Close();

		Map->LoadTiles(FireT);

		gLoc[10].x[0]=6;
		gLoc[10].y[0]=4;

		gLoc[10].x[1]=0;
		gLoc[10].y[1]=2;

		gLoc[10].x[2]=9;
		gLoc[10].y[2]=2;

		gLoc[10].x[3]=5;
		gLoc[10].y[3]=10;

		gLoc[10].x[4]=10;
		gLoc[10].y[4]=12;

		gLoc[10].x[5]=12;
		gLoc[10].y[5]=2;

		pac.SetMap(Map);
		pac.SetStartPos(9, 5);
		pac.SetPassages(4, 5);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(3, 5);
			ghost[i].SetPassages(4, 5);	
		}
	}

	if(level==12)
	{
		Resource->Open("data.wad");			
		char *fireMap2R=	Resource->GetFile("fire2.map");	
		Map->Load(fireMap2R);
		Resource->Close();

		Map->LoadTiles(FireT);

		gLoc[11].x[0]=5;
		gLoc[11].y[0]=5;

		gLoc[11].x[1]=3;
		gLoc[11].y[1]=2;

		gLoc[11].x[2]=12;
		gLoc[11].y[2]=1;

		gLoc[11].x[3]=3;
		gLoc[11].y[3]=10;

		gLoc[11].x[4]=9;
		gLoc[11].y[4]=8;

		gLoc[11].x[5]=11;
		gLoc[11].y[5]=8;

		pac.SetMap(Map);
		pac.SetStartPos(8, 5);
		pac.SetPassages(7, 5);

		for(i=0;i<4;i++)
		{
			ghost[i].SetIndex(i);
			ghost[i].SetMap(Map);	
			ghost[i].SetHideout(6, 6);
			ghost[i].SetPassages(7, 5);	
		}
	}
}

void GameWindow::RestartLevel()
{
	pc6.Reset();

	Paccy->SetFrame(24);	
	
	pac.chase=false;
	
	if(fullVersion)
	{
		if(level==1)
			pac.SetStartPos(9, 9);

		if(level==2)
			pac.SetStartPos(8, 6);

		if(level==3)
			pac.SetStartPos(8, 8);

		if(level==4)
			pac.SetStartPos(6, 5);

		if(level==5)
			pac.SetStartPos(8, 7);

		if(level==6)
			pac.SetStartPos(6, 4);

		if(level==7)
			pac.SetStartPos(7, 7);

		if(level==8)
			pac.SetStartPos(8, 7);

		if(level==9)
			pac.SetStartPos(6, 3);

		if(level==10)
			pac.SetStartPos(4, 6);

		if(level==11)
			pac.SetStartPos(9, 5);

		if(level==12)
			pac.SetStartPos(8, 5);
	}

	else
	{
		if(level==1)
			pac.SetStartPos(9, 9);

		if(level==2)
			pac.SetStartPos(6, 5);

		if(level==3)
			pac.SetStartPos(6, 4);

		if(level==4)
			pac.SetStartPos(6, 3);		
	}
					
	for(i=0;i<4;i++)
	{
		ghost[i].Reset();		
	}

	paccyAlive=true;
	pac.alive=true;
}

void GameWindow::DrawGadgets()
{
	//Workaround für die nördlichen Map-Ausbuchtungen

	if(level==2&&fullVersion)//zweiter Forest-Level
	{
		Ghost[4]->SetFrame(45);

		for(i=208;i<300;i+=25)
		{
			Ghost[4]->SetPos(i,18 );
			Ghost[4]->DrawTrans(Screen->GetBack());
		}
	}

	if(level==5)//erster City-Level
	{
		Ghost[4]->SetFrame(46);

		for(i=145;i<230;i+=25)
		{
			Ghost[4]->SetPos(i,18 );
			Ghost[4]->DrawTrans(Screen->GetBack());
		}
	}

	//zweiter City-Level
	if((level==6&&fullVersion)||(level==3&&fullVersion==false))
	{
		Ghost[4]->SetFrame(46);

		for(i=365;i<500;i+=25)
		{
			Ghost[4]->SetPos(i,18 );
			Ghost[4]->DrawTrans(Screen->GetBack());
		}

		Ghost[4]->SetPos(496,18 );
		Ghost[4]->DrawTrans(Screen->GetBack());
	}

	if(level==12)//zweiter Fire-Level
	{
		Ghost[4]->SetFrame(47);

		for(i=365;i<500;i+=25)
		{
			Ghost[4]->SetPos(i,18 );
			Ghost[4]->DrawTrans(Screen->GetBack());
		}

		Ghost[4]->SetPos(496,18 );
		Ghost[4]->DrawTrans(Screen->GetBack());
	}

	Ghost[4]->SetFrame(44);

	for(i=0;i<550;i+=25)
	{
		Ghost[4]->SetPos(i,0 );
		Ghost[4]->DrawTrans(Screen->GetBack());
	}

	Ghost[4]->SetPos(496,0 );
	Ghost[4]->DrawTrans(Screen->GetBack());

	DrawDebugText(7, 0, "Level "+IntToStr(level));
	DrawDebugText(60, 0, "Score: "+IntToStr(points));	
	
	Gadget->SetFrame(10);	//das Gauge

	int bars;
	
	if(pac.chase&&pac.doped)
	{
		if((9-(pc3.GetValue()/1000+1))>(9-(pc5.GetValue()/1000+1)))

			bars=9-(pc3.GetValue()/1000+1);
		else
			bars=9-(pc5.GetValue()/1000+1);
	}

	else
		if(pac.chase)
			bars=9-(pc3.GetValue()/1000+1);
	else
		if(pac.doped)
			bars=9-(pc5.GetValue()/1000+1);
	else
		bars=0;

	for(i=0;i<bars;i++)
	{
		Gadget->SetPos(150+i*8, 2);
		if(bars>0&&pac.alive&&pills>0)
			Gadget->DrawTrans(Screen->GetBack());
	}

	
	Gadget->SetFrame(1);	//das Herz

	for(i=1;i<pac.lives+1;i++)
	{
		Gadget->SetPos(275+i*18, 2);
		if(pac.lives>0)
			Gadget->DrawTrans(Screen->GetBack());
	}
	
	if(diamonds>0)
	{
		Gadget->SetFrame(11);	//Diamanten
		Gadget->SetPos(240, 2);		
		Gadget->DrawTrans(Screen->GetBack());
		DrawDebugText(255, 0, "x "+IntToStr(diamonds));
	}

	if(ghost[0].lives>0)
	{
		Gadget->SetFrame(2);	//Ghost 1
		Gadget->SetPos(362, 2);		
		Gadget->DrawTrans(Screen->GetBack());
		DrawDebugText(377, 0, "x "+IntToStr(ghost[0].lives));
	}

	if(ghost[1].lives>0)
	{
		Gadget->SetFrame(3);	//Ghost 2
		Gadget->SetPos(402, 2);		
		Gadget->DrawTrans(Screen->GetBack());
		DrawDebugText(417, 0, "x "+IntToStr(ghost[1].lives));
	}

	if(ghost[2].lives>0)
	{
		Gadget->SetFrame(4);	//Ghost 3
		Gadget->SetPos(442, 2);		
		Gadget->DrawTrans(Screen->GetBack());
		DrawDebugText(457, 0, "x "+IntToStr(ghost[2].lives));
	}

	if(ghost[3].lives>0)
	{
		Gadget->SetFrame(5);	//Ghost 4
		Gadget->SetPos(482, 2);		
		Gadget->DrawTrans(Screen->GetBack());
		DrawDebugText(497, 0, "x "+IntToStr(ghost[3].lives));
	}

	if(keyVisible)
	{		
		Gadget->SetFrame(0);	//Schlüssel
		Gadget->SetPos(gLoc[level-1].x[0]*32+24, gLoc[level-1].y[0]*32+40);		
		Gadget->DrawTrans(Screen->GetBack());
	}

	if(diamondVisible)
	{
		Gadget->SetFrame(11);	//Diamant
		Gadget->SetPos(gLoc[level-1].x[5]*32+31, gLoc[level-1].y[5]*32+35);		
		Gadget->DrawTrans(Screen->GetBack());
	}

	if(gadgetActive)
	{
		Gadget->SetFrame(curGadget);	//Frucht

		Gadget->SetPos(curGadgetX*32+22, curGadgetY*32+40);	
		
		Gadget->DrawTrans(Screen->GetBack());
	}

/*
	//ZUM TESTEN, alle Früchte///////

	Gadget->SetFrame(6);	//Frucht
	Gadget->SetPos(gLoc[level-1].x[1]*32+22, gLoc[level-1].y[1]*32+36);			
	Gadget->DrawTrans(Screen->GetBack());

	Gadget->SetFrame(7);	//Frucht
	Gadget->SetPos(gLoc[level-1].x[2]*32+22, gLoc[level-1].y[2]*32+36);			
	Gadget->DrawTrans(Screen->GetBack());

	Gadget->SetFrame(8);	//Frucht
	Gadget->SetPos(gLoc[level-1].x[3]*32+22, gLoc[level-1].y[3]*32+36);			
	Gadget->DrawTrans(Screen->GetBack());

	Gadget->SetFrame(9);	//Frucht
	Gadget->SetPos(gLoc[level-1].x[4]*32+22, gLoc[level-1].y[4]*32+36);			
	Gadget->DrawTrans(Screen->GetBack());
*/
}

void GameWindow::PlaceGadgets()
{
	int w1=18000;
	int w2=9200;
	
	static int wait1=Random(w1)+w1;	//bis Schlüssel erscheint
	static int wait2=Random(w1)+w1;	//bis Gadget erscheint	
	int place;

	bool upperHalf=false;					//ist Paccy in oberer Hälfte?
	if(pac.locY<7)	upperHalf=true;

	//Schlüssel erscheint

	if((int)pc5.GetValue()>wait1&&keyVisible==false
		&&pac.hasKey==false)
	{	
		wait1=Random(w1)+w1;
		keyVisible=true;				
	}

	//Doped-Zustand nach x Sekunden beenden

	if(pac.doped&&(int)pc5.GetValue()>w2)
		pac.doped=false;

	//Gadget nach x Sekunden wieder verschwinden lassen

	if((int)pc5.GetValue()>w1+waited&&gadgetActive==true)
	{
		gadgetActive=false;
		pc5.Reset();
	}

	//Wo erscheint das nächste Gadget?
	//Max. 3 Gadgets pro Level

	if((int)pc5.GetValue()>wait2&&gadgetActive==false
		&&givenGadgets<3)
	{
		givenGadgets++;

		waited=wait2;

		wait2=Random(w1)+w1;

		if(upperHalf==false)	//Paccy ist unten
		{
			place=Random(2);

			if(place==0)
			{
				curGadgetX=gLoc[level-1].x[1];
				curGadgetY=gLoc[level-1].y[1];
			}

			else
			{
				curGadgetX=gLoc[level-1].x[2];
				curGadgetY=gLoc[level-1].y[2];
			}
		}

		else	//Paccy ist oben
		{
			place=Random(2);

			if(place==0)
			{
				curGadgetX=gLoc[level-1].x[3];
				curGadgetY=gLoc[level-1].y[3];
			}

			else
			{
				curGadgetX=gLoc[level-1].x[4];
				curGadgetY=gLoc[level-1].y[4];
			}
		}			

		curGadget=Random(4)+6;	//welche der 4 Früchte?

		gadgetActive=true;		
	}
}

void GameWindow::PlayIntro()
{
	int v;	
	static int x=-100;
	static int y=224;

	static bool moveRight=true;	
	static bool move1=false;
	static bool move2=false;		

	Map->MoveTo(27,3);

	if(move1==false&&move2==false)	
		Map->Fill(1);	
	
	else	
		Map->Fill(2);		

	if(pc2.GetValue()>6)	
	{
		v=1;
		pc2.Reset();
	}
	else v=0;	

	static int wrapX=0;
	static int scroll=0;

	scroll++;

	if(scroll>10)
	{
		scroll=0;
		wrapX+=1;
	}

	if(moveRight)
	{
		if(move1||move2)			
			Map->WrapScrollDown(wrapX);		

		x+=v;

		if(move1==false)
		{
			if(x>530&&move1==false&&move2==false)
				moveRight=false;		
		}
	}
	else 
	{	
		x-=v;

		if(x<-100)
		{
			move1=true;
			moveRight=true;
		}
	}							

	//Animationsphasen

	if(pc1.GetValue()>450)	pc1.Reset();	

	if(move1==false&&move2==false)
	{
		if(pc1.GetValue()>300)			
		{
			if(moveRight)		Paccy->SetFrame(5);
			else				Paccy->SetFrame(2);					 
		}

		else if(pc1.GetValue()>150)			
		{	
			if(moveRight)		Paccy->SetFrame(4);
			else				Paccy->SetFrame(1);	
		}
		
		else 
		{	
			if(moveRight)		Paccy->SetFrame(3);
			else				Paccy->SetFrame(0);		
		}

		if(pc1.GetValue()>225)
		{
			if(moveRight)
			{
				Ghost[0]->SetFrame(2);
				Ghost[1]->SetFrame(10);
				Ghost[2]->SetFrame(18);
				Ghost[3]->SetFrame(26);
			}
			else
			{
				Ghost[0]->SetFrame(32);		
				Ghost[1]->SetFrame(32);		
				Ghost[2]->SetFrame(32);		
				Ghost[3]->SetFrame(32);		
			}
		}

		else
		{
			if(moveRight)
			{
				Ghost[0]->SetFrame(3);
				Ghost[1]->SetFrame(11);
				Ghost[2]->SetFrame(19);
				Ghost[3]->SetFrame(27);
			}
			else
			{
				Ghost[0]->SetFrame(33);		
				Ghost[1]->SetFrame(33);		
				Ghost[2]->SetFrame(33);		
				Ghost[3]->SetFrame(33);		
			}
		}
	}	

	Map->DrawClipped(Screen->GetBack(),&rect);
	
	if(move1==false&&move2==false)
	{
		Paccy->SetPos(x, y);
		Paccy->DrawClipped(Screen->GetBack(), &rect);

		
		Ghost[0]->SetPos(x-80, y);
		Ghost[1]->SetPos(x-130, y);
		Ghost[2]->SetPos(x-180, y);
		Ghost[3]->SetPos(x-230, y);
		
		Ghost[0]->DrawClipped(Screen->GetBack(),&rect);
		Ghost[1]->DrawClipped(Screen->GetBack(),&rect);
		Ghost[2]->DrawClipped(Screen->GetBack(),&rect);
		Ghost[3]->DrawClipped(Screen->GetBack(),&rect);
	}

	else if(move1)
	{
		Intro->SetFrame(0);
		Intro->SetPos(80,x);
		Intro->DrawClipped(Screen->GetBack(),&rect);

		Intro->SetFrame(1);
		Intro->SetPos(210,400-x);
		Intro->DrawClipped(Screen->GetBack(),&rect);

		if(x>200)
		{
			move1=false;
			move2=true;
			x=0;
		}
	}	

	else if(move2)
	{
		int x1;

		if(x<100)
			x1=200;
		else x1=x;

		if(x<100)
		{
			Intro->SetFrame(0);
			Intro->SetPos(80,x1);
			Intro->DrawClipped(Screen->GetBack(),&rect);

			Intro->SetFrame(1);
			Intro->SetPos(210,400-x1);
			Intro->DrawClipped(Screen->GetBack(),&rect);
		}

		if(x>=100)
		{
			int x2;
			if(x>=120)x2=120;
			else x2=x;

			Intro->SetFrame(0);
			Intro->SetPos(180-x2,200);
			Intro->DrawClipped(Screen->GetBack(),&rect);

			Intro->SetFrame(1);
			Intro->SetPos(110+x2,200);
			Intro->DrawClipped(Screen->GetBack(),&rect);			
		}

		if(x>=120&&x<=600)
		{
			static bool played=false;

			if(x<590&&played==false)
			{
				if(sound>0)TriumphSound->Play();
				played=true;
			}

			DrawDebugText(125,250,"© 2000 by Johannes Wallroth - www.programming.de");

			Paccy->SetPos(195,210);

			if(x>330)Paccy->SetFrame(29);
			else if(x>290)Paccy->SetFrame(28);
			else if(x>250)Paccy->SetFrame(27);
			else if(x>210)Paccy->SetFrame(26);
			else if(x>170)Paccy->SetFrame(25);
			else Paccy->SetFrame(24);
			
			Paccy->DrawClipped(Screen->GetBack(),&rect);						
		}

		if(x>600)
		{
			intro=false;
			Map->MoveTo(12,-8);
			NewGame();
		}
	}

	Screen->Flip();
}

void GameWindow::ShowTitle()
{
	if(fullVersion)
	{
		if(level==1||level==2)Titles->SetFrame(0);	//Forest
		if(level==3||level==4)Titles->SetFrame(1);	//Ice
		if(level==5||level==6)Titles->SetFrame(2);	//City
		if(level==7||level==8)Titles->SetFrame(3);	//Desert
		if(level==9||level==10)Titles->SetFrame(4);	//Night
		if(level==11||level==12)Titles->SetFrame(5);//Fire	
	}

	else
	{
		if(level==1)Titles->SetFrame(0);	
		if(level==2)Titles->SetFrame(1);
		if(level==3)Titles->SetFrame(2);
		if(level==4)Titles->SetFrame(4);		
	}


	Titles->SetPos(20,30);
			
	if(pc6.GetValue()<3000&&pac.lives==3)
	{		
		Titles->DrawTrans(Screen->GetBack());			
	}			
}

//Die aufsteigenden Punkte

void GameWindow::DrawPoints(int p)
{
	int offset;
	if(p>99)offset=22;
	else offset=25;

	if(pc7.GetValue()<1000&&p>0)		
		DrawDebugText(pac.x+offset,
		pac.y+30-pc7.GetValue()/50, IntToStr(p));
}

void GameWindow::TriggerPopUpPoints(int p)
{
	popPoints=p;
	pc7.Reset();	
}

void GameWindow::OnNewGame()
{
	if(intro)
	{
		Map->MoveTo(12,-8);
		intro=false;
	}

	level=1;
	NewGame();
}

void GameWindow::OnSettings()
{
	CSettings settings;
	
	settings.sound=sound;	

	if(settings.DoModal()==IDOK)
	{
		sound=settings.sound;	
	};
}

void GameWindow::LoadSettings()
{
	sound=::GetPrivateProfileInt(
		"PacWorld", "Sound", 2, "PacWorld.ini");

	highscore=::GetPrivateProfileInt(
		"PacWorld", "Highscore", 0, "PacWorld.ini");

	char txt[25];

	::GetPrivateProfileString (
		"PacWorld", "Password",
		"", txt, 25, "PacWorld.ini");

	password=txt;
}

void GameWindow::SaveSettings()
{
	::WritePrivateProfileString(
		"PacWorld", "Sound", IntToStr(sound), "PacWorld.ini");

	if(highscore>points)
	::WritePrivateProfileString(
		"PacWorld", "Highscore", IntToStr(highscore), "PacWorld.ini");
	
	::WritePrivateProfileString(
		"PacWorld", "Password", password, "PacWorld.ini");
}


void GameWindow::OnPause()
{
	if(intro==false)paused^=true;

	//Häkchen vor Pausemenü setzen
	CMenu* pMenu = GetMenu()->GetSubMenu(0);
	
	if(paused)
		pMenu->CheckMenuItem( ID_GAME_PAUSE, MF_CHECKED );

	else
		pMenu->CheckMenuItem( ID_GAME_PAUSE, MF_UNCHECKED );
}
