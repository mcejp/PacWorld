#ifndef AFX_GAMEWINDOW_H__230532E0_90FB_11D2_91E6_081A0CC10E27__INCLUDED_
#define AFX_GAMEWINDOW_H__230532E0_90FB_11D2_91E6_081A0CC10E27__INCLUDED_

// GameWindow.h : Header-Datei
//
#define CDXINCLUDEALL
#include <CDX.H>
#include "cdxhpc.h"
#include "Paccy.h"
#include "Ghost.h"

/////////////////////////////////////////////////////////////////////////////
// Rahmen GameWindow 

class GameWindow : public CFrameWnd
{
	
// Operationen
private:
	void OnPause();
	bool paused;
	void Set4LevelInfo();
	CString password;
	bool fullVersion;
	int score, highscore;
	int speed;
	void SaveSettings();
	void LoadSettings();
	void OnSettings();
	int sound;
	void OnNewGame();
	void TriggerPopUpPoints(int i);
	int popPoints;
	void DrawPoints(int p);
	int points;
	void ShowTitle();
	void OnTutorial();
	bool intro;
	void PlayIntro();
	bool diamondCollected;
	int diamonds;
	bool diamondVisible;
	void PlaceGadgets();

	struct gadgetLocation 
	{
		int x[6];
		int y[6];	//Koordinaten für Gadgets		
	}gLoc[12];	

	int givenGadgets;	//bereits erhaltene Gadgets

	int curGadget;	//das aktuell erschienene Gadget, 1-4

	bool keyVisible;

	bool gadgetActive;

	int waited;
	
	int curGadgetX, curGadgetY;	//aktuelle Location von Gadget

	void DrawGadgets();
	void RestartLevel();
	int level;
	void SetLevelInfo();
	bool firstKeyPress;
	void NewGame();
	bool pillEaten;
	bool fired3;
	int pills;	
	bool paccyAlive;
	void EatCookies();
	int i, j;

	CPaccy pac;
	CGhost ghost[4];

	int moving;	

	void SetAnimationFrames();
	void DrawDebugText(int x, int y, CString s);
	CDXHPC pc1, pc2, pc3, pc4, pc5, pc6, pc7;

	RECT rect;
	
	void DrawStuff();

// Attribute
public:
	CDXTile *ForestT, *IceT, *CityT, *DesertT, *NightT, *FireT,
		*IntroT;
	CDXMap *Map;	
	CDXSprite *Paccy, *Intro;
	CDXSprite *Ghost[5];
	CDXSprite *Gadget, *Titles;
	CDXScreen *Screen;
	CDXSound *Sound;
	CDXSoundBuffer *ChompSound, *CatchSound, *FoundSound, 
		*KilledSound, *ItemSound, *TriumphSound;
	CDXResource* Resource;
	
	GameWindow(LPCSTR ClassName);

	BOOL InitDirectDraw();

	///called from OnIdle()/////
	void MoveGame();

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(GameWindow)
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~GameWindow();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(GameWindow)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);	
	afx_msg void OnHelpAbout();
	afx_msg void OnGameExit();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_GAMEWINDOW_H__230532E0_90FB_11D2_91E6_081A0CC10E27__INCLUDED_
