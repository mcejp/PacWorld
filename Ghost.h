// Ghost.h: Schnittstelle für die Klasse CGhost.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GHOST_H__906E8DE0_B882_11D3_A5E4_B5590424C036__INCLUDED_)
#define AFX_GHOST_H__906E8DE0_B882_11D3_A5E4_B5590424C036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CDX.H>
#include "cdxhpc.h"


class CGhost  
{
private:
	int index;
	CDXHPC pc1, pc2;
	int paccyX, paccyY;
	bool chased;
	int leftPassage, rightPassage;
	bool newDir;
	int goWhere;	
	int dir;
	int x,y,v;
	int hideoutY;
	int hideoutX;
	bool up, down, left, right;
	bool goUp, goDown, goLeft, goRight;	
	void StartGhost();
	CDXMap* Map;

public:	
	bool onWayHome;
	void MoveHome();
	bool vulnerable;
	bool startingPhase;
	void Reset();
	void SetIndex(int index);
	bool alive;
	void Kill();
	int lives;
	bool isNear;
	void SetPaccyState(int x, int y, bool ch);
	void SetPassages(int y1, int y2);
	int GetY();
	int GetX();
	bool started;	
	void SetHideout(int x, int y);	
	void MoveGhost();
	int moving;	
	void SetMap(CDXMap *map);	
	
	CGhost();
	virtual ~CGhost();

};

#endif // !defined(AFX_GHOST_H__906E8DE0_B882_11D3_A5E4_B5590424C036__INCLUDED_)
