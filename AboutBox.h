#ifndef AFX_ABOUTBOX_H__230532E2_90FB_11D2_91E6_081A0CC10E27__INCLUDED_
#define AFX_ABOUTBOX_H__230532E2_90FB_11D2_91E6_081A0CC10E27__INCLUDED_

// AboutBox.h : Header-Datei
//
#include "hyperlink.h"
#include "BtnST.h"
#include "ColorStaticST.h"
/////////////////////////////////////////////////////////////////////////////
// Dialogfeld AboutBox 

class AboutBox : public CDialog
{
// Konstruktion
public:
	AboutBox(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(AboutBox)
	enum { IDD = ABOUTDIALOG };
	CButtonST	m_ok;
	CHyperLink	m_url;	
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(AboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(AboutBox)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_ABOUTBOX_H__230532E2_90FB_11D2_91E6_081A0CC10E27__INCLUDED_
