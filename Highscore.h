#if !defined(AFX_HIGHSCORE_H__556749A0_C3AC_11D3_A5E4_F529A545CC35__INCLUDED_)
#define AFX_HIGHSCORE_H__556749A0_C3AC_11D3_A5E4_F529A545CC35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Highscore.h : Header-Datei
//
#include "ColorStaticST.h"
#include "BtnST.h"
/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHighscore 

class CHighscore : public CDialog
{
private:
	CFont font1;
// Konstruktion
public:
	int highscore;
	int score;
	CHighscore(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CHighscore)
	enum { IDD = IDD_HIGHSCORE };
	CButtonST	okButton;
	CColorStaticST	scoreStatic;
	CColorStaticST	highscoreStatic;
	CString	highscoreText;
	CString	scoreText;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CHighscore)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CHighscore)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_HIGHSCORE_H__556749A0_C3AC_11D3_A5E4_F529A545CC35__INCLUDED_
