// Settings.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "resource.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettings 


CSettings::CSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettings)
	//}}AFX_DATA_INIT
}


void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettings)
		DDX_Control(pDX, IDC_RADIO1, m_radio1);
		DDX_Control(pDX, IDC_RADIO2, m_radio2);
		DDX_Control(pDX, IDC_RADIO3, m_radio3);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettings, CDialog)
	//{{AFX_MSG_MAP(CSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettings 

BOOL CSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if(sound==0)m_radio3.SetCheck(1);	
	if(sound==1)m_radio2.SetCheck(1);	
	if(sound==2)m_radio1.SetCheck(1);	

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CSettings::OnOK() 
{
	if(m_radio1.GetCheck()==1)sound=2;	
	if(m_radio2.GetCheck()==1)sound=1;	
	if(m_radio3.GetCheck()==1)sound=0;	


	CDialog::OnOK();
}
