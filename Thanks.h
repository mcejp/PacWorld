#if !defined(AFX_THANKS_H__556749A2_C3AC_11D3_A5E4_F529A545CC35__INCLUDED_)
#define AFX_THANKS_H__556749A2_C3AC_11D3_A5E4_F529A545CC35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Thanks.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CThanks 

class CThanks : public CDialog
{
// Konstruktion
public:
	bool correct;
	CThanks(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CThanks)
	enum { IDD = IDD_THANKS };
	CString	m_text;
	CString	m_text1;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CThanks)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CThanks)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_THANKS_H__556749A2_C3AC_11D3_A5E4_F529A545CC35__INCLUDED_
