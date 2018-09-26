// BuyVPNDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Config.h"
#include "Common.h"
#include "HttpWorker.h"
#include "BuyVPN.h"
#include "BuyVPNDlg.h"
#include "unzip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static UINT BASED_CODE indicators[] =
{
	IDS_INDICATOR1,
	IDS_INDICATOR2
};


CBuyVPNDlg::CBuyVPNDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBuyVPNDlg::IDD, pParent), m_brush((COLORREF)0x3a3a3a)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hConnected = AfxGetApp()->LoadIcon(IDI_CONNECTED);
	m_hDisconnected = AfxGetApp()->LoadIcon(IDI_DISCONNECTED);
	m_fNeedClose = FALSE;
	m_pHostsConfig = new CHostsConfig();
	m_pOptions = new COptions();
	m_pMembersAreaFont = NULL;
	m_pConfigurations = new CConfigurations();
	m_pNetAdapters = new CNetAdapters();
	m_pRunThread = new CRunThread(m_pOptions, m_pHostsConfig, m_pConfigurations, m_pNetAdapters, this);
	m_dwState = VVC_STATE_DISCONNECTED;
	m_strBalance = TEXT("-");
	m_strExpires = TEXT("-");
}

void CBuyVPNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOGIN, m_edUsername);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edPassword);
	DDX_Control(pDX, IDC_CHECK_SAVELOGIN, m_chbSavePass);
	DDX_Control(pDX, IDC_COMBO_CONFIGURATION, m_cobConfiguration);
	DDX_Control(pDX, IDC_COMBO_NETWORK, m_cobNetAdapter);
	DDX_Control(pDX, IDC_CHECK_ACCOUNT_STATE, m_chbCheckAccount);
	//DDX_Control(pDX, IDC_CHECK_ACCOUNT_STATE2, m_chbCheckAccount2);
	//DDX_Control(pDX, IDC_CHECK_ACCOUNT_STATE3, m_chbCheckAccount3);
	//DDX_Control(pDX, IDC_CHECK_LAUNCH_ONSTART, m_chbLaunchOnStart);
	DDX_Control(pDX, IDC_STATIC_MEMBER_AREA, m_stMemberArea);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BUTTON_SHOWLOG, m_btnShowLog);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_stLogo);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_btnMin);
	DDX_Control(pDX, IDC_STATIC_STATUS_TEXT, m_stStatusText);
	DDX_Control(pDX, IDC_STATIC_BALANCE_TEXT, m_stBalanceText);
	DDX_Control(pDX, IDC_STATIC_EXPIRES_TEXT, m_stExpiresText);
	//	DDX_Control(pDX, IDC_RADIO_SINGLE_VPN, m_rbSingleVpn);
	DDX_Control(pDX, IDC_RADIO_SINGLE_VPN, m_rbSingleVpn);
	DDX_Control(pDX, IDC_RADIO_DOUBLE_VPN, m_rbDoubleVpn);
	DDX_Control(pDX, IDC_RADIO_TRIPLE_VPN, m_rbTripleVpn);
	DDX_Control(pDX, IDC_RADIO_QUADRO_VPN, m_rbQuadroVpn);
	DDX_Control(pDX, IDC_RADIO_PENTA_VPN, m_rbPentaVpn);
	DDX_Control(pDX, IDC_BUTTON_ENGLISH, m_btnEng);
	DDX_Control(pDX, IDC_BUTTON_RUSSIAN, m_btnRus);
}

BEGIN_MESSAGE_MAP(CBuyVPNDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_NOTIFYICON, OnTrayIcon)
	ON_MESSAGE(WM_CHANGESTATE, OnChangeState)
	ON_MESSAGE(WM_UPDATEACCINFO, OnUpdateAccInfo)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CBuyVPNDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CBuyVPNDlg::OnBnClickedButtonExit)
	ON_EN_CHANGE(IDC_EDIT_LOGIN, &CBuyVPNDlg::OnEnChangeEditLogin)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CBuyVPNDlg::OnEnChangeEditPassword)
	ON_BN_CLICKED(IDC_CHECK_SAVELOGIN, &CBuyVPNDlg::OnBnClickedCheckSavelogin)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT_STATE, &CBuyVPNDlg::OnBnClickedCheckAccountState)
	//ON_BN_CLICKED(IDC_CHECK_LAUNCH_ONSTART, &CBuyVPNDlg::OnBnClickedCheckLaunchOnstart)
	ON_STN_CLICKED(IDC_STATIC_MEMBER_AREA, &CBuyVPNDlg::OnStnClickedStaticMemberArea)
	ON_STN_DBLCLK(IDC_STATIC_MEMBER_AREA, &CBuyVPNDlg::OnStnClickedStaticMemberArea)
	ON_CBN_SELCHANGE(IDC_COMBO_CONFIGURATION, &CBuyVPNDlg::OnCbnSelchangeComboConfiguration)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CBuyVPNDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_SHOWLOG, &CBuyVPNDlg::OnBnClickedButtonShowlog)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CBuyVPNDlg::OnBnClickedButtonMin)
	ON_CBN_SELCHANGE(IDC_COMBO_NETWORK, &CBuyVPNDlg::OnCbnSelchangeComboNetwork)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT_STATE2, &CBuyVPNDlg::OnBnClickedCheckAccountState2)
	ON_BN_CLICKED(IDC_RADIO_SINGLE_VPN, &CBuyVPNDlg::OnBnClickedRadioSingleVpn)
	ON_BN_CLICKED(IDC_RADIO_DOUBLE_VPN, &CBuyVPNDlg::OnBnClickedRadioDoubleVpn)
	ON_BN_CLICKED(IDC_RADIO_TRIPLE_VPN, &CBuyVPNDlg::OnBnClickedRadioTripleVpn)
	ON_BN_CLICKED(IDC_RADIO_QUADRO_VPN, &CBuyVPNDlg::OnBnClickedRadioQuadroVpn)
	ON_BN_CLICKED(IDC_RADIO_PENTA_VPN, &CBuyVPNDlg::OnBnClickedRadioPentaVpn)
END_MESSAGE_MAP()


// CBuyVPNDlg message handlers

BOOL CBuyVPNDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText(VC_PROGRAM_NAME);

	//Logo
	//m_Logo.SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_LOGO)));

	//Buttons
	m_btnConnect.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnConnect.SetFlat(TRUE, FALSE);
	m_btnConnect.DrawBorder(FALSE, FALSE);
	m_btnConnect.SetBtnCursor(32649, FALSE);
	m_btnConnect.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnConnect.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,255,100), FALSE);
	m_btnConnect.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnConnect.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255), FALSE);
	m_btnConnect.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnConnect.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,100), FALSE);
	m_btnConnect.SetBitmaps(IDB_BIGGREENHL, RGB(0x3a,0x3a,0x3a), IDB_BIGGREEN, RGB(0x3a,0x3a,0x3a));

	m_btnEdit.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnEdit.SetFlat(TRUE, FALSE);
	m_btnEdit.DrawBorder(FALSE, FALSE);
	m_btnEdit.SetBtnCursor(32649, FALSE);
	m_btnEdit.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnEdit.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,255,100), FALSE);
	m_btnEdit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnEdit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255), FALSE);
	m_btnEdit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnEdit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,100), FALSE);
	m_btnEdit.SetBitmaps(IDB_MIDBLUEHL, RGB(0x3a,0x3a,0x3a), IDB_MIDBLUE, RGB(0x3a,0x3a,0x3a));

	m_btnShowLog.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnShowLog.SetFlat(TRUE, FALSE);
	m_btnShowLog.DrawBorder(FALSE, FALSE);
	m_btnShowLog.SetBtnCursor(32649, FALSE);
	m_btnShowLog.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnShowLog.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,255,100), FALSE);
	m_btnShowLog.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnShowLog.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255), FALSE);
	m_btnShowLog.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnShowLog.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,100), FALSE);
	m_btnShowLog.SetBitmaps(IDB_MIDBLUEHL, RGB(0x3a,0x3a,0x3a), IDB_MIDBLUE, RGB(0x3a,0x3a,0x3a));

	m_btnExit.SetFlat(TRUE, FALSE);
	m_btnExit.DrawBorder(FALSE, FALSE);
	m_btnExit.SetBtnCursor(32649, FALSE);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnExit.SetBitmaps(IDB_CLOSEHL, RGB(0x3a,0x3a,0x3a), IDB_CLOSE, RGB(0x3a,0x3a,0x3a));

	m_btnMin.SetFlat(TRUE, FALSE);
	m_btnMin.DrawBorder(FALSE, FALSE);
	m_btnMin.SetBtnCursor(32649, FALSE);
	m_btnMin.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnMin.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnMin.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x3a,0x3a,0x3a), FALSE);
	m_btnMin.SetBitmaps(IDB_MINHL, RGB(0x3a,0x3a,0x3a), IDB_MIN, RGB(0x3a,0x3a,0x3a));
	

	m_btnEng.SetFlat(TRUE, FALSE);
	m_btnEng.DrawBorder(FALSE, FALSE);
	m_btnEng.SetBtnCursor(32649, FALSE);
	m_btnEng.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x3a, 0x3a, 0x3a), FALSE);
	m_btnEng.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x3a, 0x3a, 0x3a), FALSE);
	m_btnEng.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x3a, 0x3a, 0x3a), FALSE);
	m_btnEng.SetBitmaps(IDB_ENGHL, RGB(0x3a, 0x3a, 0x3a), IDB_ENG, RGB(0x3a, 0x3a, 0x3a));

	m_btnRus.SetFlat(TRUE, FALSE);
	m_btnRus.DrawBorder(FALSE, FALSE);
	m_btnRus.SetBtnCursor(32649, FALSE);
	m_btnRus.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x3a, 0x3a, 0x3a), FALSE);
	m_btnRus.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x3a, 0x3a, 0x3a), FALSE);
	m_btnRus.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x3a, 0x3a, 0x3a), FALSE);
	m_btnRus.SetBitmaps(IDB_RUSHL, RGB(0x3a, 0x3a, 0x3a), IDB_RUS, RGB(0x3a, 0x3a, 0x3a));


	//Загрузка настроек
	m_pOptions->Load();

	//Проверяем на наличие новых хостов, версий программы и конфигов 
	CString strConfigVersion;
	CString strConfigUrl;
	m_pHostsConfig->Update(strConfigVersion, strConfigUrl);
;
	BOOL isConfigListEmpty = (BOOL)(m_pConfigurations->GetNameList()->GetCount() == 0);
	CheckConfig(isConfigListEmpty, strConfigVersion, strConfigUrl);

	//Настройка Member's area
	int pixelsPerInch = this->GetDC()->GetDeviceCaps(LOGPIXELSY);
	int fontHeight = -MulDiv(9, pixelsPerInch, 72);
	m_pMembersAreaFont = new CFont();
	m_pMembersAreaFont->CreateFont(fontHeight, 0, 0, 0, FW_NORMAL, FALSE, TRUE, 0, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));
	m_stMemberArea.SetFont(m_pMembersAreaFont, TRUE);
	m_curHand = LoadCursor(NULL, IDC_HAND);


	
	//Получение списка сетевых адаптеров
	m_pNetAdapters->Update();
	CList<CString, CString> *pList = m_pNetAdapters->GetNameList();
	POSITION pos = pList->GetHeadPosition();
	while (pos)
	{
		m_cobNetAdapter.AddString(pList->GetNext(pos));
	}

	//Применение настроек
	ApplyOptions();



	ChangeState(VVC_STATE_DISCONNECTED);

	//Добавлем иконку в трей и загружаем меню для нее
	m_TrayMenu.LoadMenu(IDR_TRAY_MENU);
	AddNotifyIcon();

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CBuyVPNDlg::UpdateConfigurations()
{
	m_cobConfiguration.ResetContent();
	if (m_rbSingleVpn.GetCheck())
	{
		m_pOptions->m_strFolder = L"Single";		
	}
	else if (m_rbDoubleVpn.GetCheck())
	{
		m_pOptions->m_strFolder = L"Double";
	}
	else if (m_rbTripleVpn.GetCheck())
	{
		m_pOptions->m_strFolder = L"Triple";
	}
	else if (m_rbQuadroVpn.GetCheck())
	{
		m_pOptions->m_strFolder = L"Quadro";
	}
	else if (m_rbPentaVpn.GetCheck())
	{
		m_pOptions->m_strFolder = L"Penta";
	}

	m_pOptions->Save(VCOF_FOLDER);
	m_pConfigurations->Update(m_pOptions->m_strFolder);

	CList<CString, CString>* pList = m_pConfigurations->GetNameList();
	POSITION pos = pList->GetHeadPosition();
	while (pos)
	{
		m_cobConfiguration.AddString(pList->GetNext(pos));
	}
	m_cobConfiguration.SetCurSel(0);
}

void CBuyVPNDlg::OnDestroy()
{
	if (m_fNeedClose)
	{
		DeleteNotifyIcon();

		CDialog::OnDestroy();

		if (m_pRunThread)
		{
			m_pRunThread->Stop();
			delete m_pRunThread;
		}

		if (m_pMembersAreaFont)
			delete m_pMembersAreaFont;

		if (m_pConfigurations)
			delete m_pConfigurations;

		if (m_pNetAdapters)
			delete m_pNetAdapters;

		if (m_pOptions)
			delete m_pOptions;

		ExitProcess(0);
	}
}

void CBuyVPNDlg::OnClose()
{
	if (m_fNeedClose)
	{
		CDialog::OnClose();
	}
	else
	{
		ShowWindow(SW_MINIMIZE);
		ShowWindow(SW_HIDE);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBuyVPNDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBuyVPNDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBuyVPNDlg::AddNotifyIcon()
{
	NOTIFYICONDATA nid; 

	nid.cbSize = sizeof(NOTIFYICONDATA); 
	nid.hWnd = m_hWnd; 
	nid.uID = 1; 
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	nid.uCallbackMessage = WM_NOTIFYICON;
	nid.hIcon = m_hDisconnected; 
	lstrcpyn(nid.szTip, VC_PROGRAM_NAME, sizeof(nid.szTip));

	Shell_NotifyIcon(NIM_ADD, &nid); 
}

void CBuyVPNDlg::ModifyNotifyIcon(BOOL fConnected)
{
	NOTIFYICONDATA nid; 

	nid.cbSize = sizeof(NOTIFYICONDATA); 
	nid.hWnd = m_hWnd; 
	nid.uID = 1; 
	nid.uFlags = NIF_ICON | NIF_TIP; 
	nid.hIcon = fConnected ? m_hConnected : m_hDisconnected;
	lstrcpyn(nid.szTip, VC_PROGRAM_NAME, sizeof(nid.szTip));

	Shell_NotifyIcon(NIM_MODIFY, &nid); 
}

void CBuyVPNDlg::DeleteNotifyIcon()
{
	NOTIFYICONDATA nid; 

	nid.cbSize = sizeof(NOTIFYICONDATA); 
	nid.hWnd = m_hWnd; 
	nid.uID = 1; 
	nid.uFlags = NIF_MESSAGE | NIF_ICON; 
	nid.uCallbackMessage = WM_NOTIFYICON;
	nid.hIcon = m_hDisconnected; 

	Shell_NotifyIcon(NIM_DELETE, &nid); 
}

LRESULT CBuyVPNDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_RBUTTONDOWN:
		{
			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow();
			CMenu* pTrayMenu = m_TrayMenu.GetSubMenu(0);
			pTrayMenu->SetDefaultItem(0, TRUE);

			UINT uiState;
			if (m_dwState == VVC_STATE_DISCONNECTED)
				uiState = MF_BYCOMMAND | MF_ENABLED;
			else
				uiState = MF_BYCOMMAND | MF_DISABLED;
			pTrayMenu->EnableMenuItem(ID_TRAYMENU_CONNECT, uiState);
			if (m_dwState == VVC_STATE_CONNECTING || m_dwState == VVC_STATE_CONNECTED)
				uiState = MF_BYCOMMAND | MF_ENABLED;
			else
				uiState = MF_BYCOMMAND | MF_DISABLED;
			pTrayMenu->EnableMenuItem(ID_TRAYMENU_DISCONNECT, uiState);

			UINT uiCmd = pTrayMenu->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_RETURNCMD,
				pt.x,
				pt.y,
				this,
				NULL);

			switch (uiCmd)
			{
			case ID_TRAYMENU_SHOW:
				ShowWindow(SW_SHOW);
				ShowWindow(SW_RESTORE);
				break;
			case ID_TRAYMENU_CONNECT:
			case ID_TRAYMENU_DISCONNECT:
				OnBnClickedButtonConnect();
				break;
			case ID_TRAYMENU_EXIT:
				m_fNeedClose = TRUE;
				SendMessage(WM_DESTROY, 0, 0);
				break;
			}

			return TRUE;
		}
	case WM_LBUTTONDBLCLK:
		{
			ShowWindow(SW_SHOW);
			ShowWindow(SW_RESTORE);
			return TRUE;
		}
	}

	return FALSE;
}

LRESULT CBuyVPNDlg::OnChangeState(WPARAM wParam, LPARAM lParam)
{
	ChangeState((DWORD)lParam);

	return TRUE;
}

void CBuyVPNDlg::OnBnClickedButtonConnect()
{
	if (m_dwState == VVC_STATE_DISCONNECTED)
	{
		ChangeState(VVC_STATE_STARTING);
		m_pRunThread->Run();
	}
	else if (m_dwState == VVC_STATE_CONNECTED || m_dwState == VVC_STATE_CONNECTING)
	{
		ChangeState(VVC_STATE_DISCONNECTING);
		m_pRunThread->Stop();
	}
}

void CBuyVPNDlg::OnBnClickedButtonExit()
{
	//ShowWindow(SW_HIDE);
	
	EndDialog(0);
}

void CBuyVPNDlg::OnBnClickedButtonMin()
{
	ShowWindow(SW_MINIMIZE);
}

void CBuyVPNDlg::ApplyOptions()
{
	if (m_pOptions == NULL)
	{
		return;
	}

	if (m_pOptions->m_fSavePassword)
	{
		m_edUsername.SetWindowText(m_pOptions->m_strUsername);
		m_edPassword.SetWindowText(m_pOptions->m_strPassword);
	}
	m_chbSavePass.SetCheck(m_pOptions->m_fSavePassword ? BST_CHECKED : BST_UNCHECKED);

	if (m_pOptions->m_strFolder.IsEmpty())
		m_pOptions->m_strFolder = L"Single";

	if (m_pOptions->m_strFolder == L"Single")
		m_rbSingleVpn.SetCheck(true);
	else if (m_pOptions->m_strFolder == L"Double")
		m_rbDoubleVpn.SetCheck(true);
	else if (m_pOptions->m_strFolder == L"Triple")
		m_rbTripleVpn.SetCheck(true);
	else if (m_pOptions->m_strFolder == L"Quadro")
		m_rbQuadroVpn.SetCheck(true);
	else if (m_pOptions->m_strFolder == L"Penta")
		m_rbPentaVpn.SetCheck(true);

	UpdateConfigurations();

	if (m_pOptions->m_strConfiguration.IsEmpty())
	{
		if (m_cobConfiguration.GetCount() > 0)
		{
			m_cobConfiguration.SetCurSel(0);
			m_cobConfiguration.GetLBText(0, m_pOptions->m_strConfiguration);
		}
		else
		{
			m_pOptions->m_strConfiguration.Empty();
		}
	}
	else
	{
		m_cobConfiguration.SelectString(0, m_pOptions->m_strConfiguration);
	}

	if (m_pOptions->m_strNetAdapter.IsEmpty())
	{
		if (m_cobNetAdapter.GetCount() > 0)
		{
			m_cobNetAdapter.SetCurSel(0);
			m_cobNetAdapter.GetLBText(0, m_pOptions->m_strNetAdapter);
		}
	}
	else
	{
		m_cobNetAdapter.SelectString(0, m_pOptions->m_strNetAdapter);
	}
	m_chbCheckAccount.SetCheck(m_pOptions->m_fCheckAccount ? BST_CHECKED : BST_UNCHECKED);
	m_chbLaunchOnStart.SetCheck(m_pOptions->m_fLaunchOnStart ? BST_CHECKED : BST_UNCHECKED);
}

void CBuyVPNDlg::OnEnChangeEditLogin()
{
	TCHAR szBuffer[256];
	ZeroMemory(szBuffer, sizeof(szBuffer));
	m_edUsername.GetWindowText(szBuffer, sizeof(szBuffer)-1);
	m_pOptions->m_strUsername = szBuffer;
	if (m_pOptions->m_fSavePassword)
	{
		m_pOptions->Save(VCOF_USERNAME);
	}
}

void CBuyVPNDlg::OnEnChangeEditPassword()
{
	TCHAR szBuffer[256];
	ZeroMemory(szBuffer, sizeof(szBuffer));
	m_edPassword.GetWindowText(szBuffer, sizeof(szBuffer)-1);
	m_pOptions->m_strPassword = szBuffer;
	if (m_pOptions->m_fSavePassword)
	{
		m_pOptions->Save(VCOF_PASSWORD);
	}
}

void CBuyVPNDlg::OnBnClickedCheckSavelogin()
{
	//m_SavePass.SetCheck(!(BOOL)m_SavePass.GetCheck());
	m_pOptions->m_fSavePassword = (BOOL)(m_chbSavePass.GetCheck() == BST_CHECKED);
	m_pOptions->Save(m_pOptions->m_fSavePassword ? VCOF_SAVEPASS | VCOF_USERNAME | VCOF_PASSWORD : VCOF_SAVEPASS);
}

void CBuyVPNDlg::OnBnClickedCheckAccountState()
{
	m_pOptions->m_fCheckAccount = (BOOL)(m_chbCheckAccount.GetCheck() == BST_CHECKED);
	m_pOptions->Save(VCOF_CHECKACCOUNT);
}

void CBuyVPNDlg::OnBnClickedCheckLaunchOnstart()
{
	m_pOptions->m_fLaunchOnStart = (BOOL)(m_chbLaunchOnStart.GetCheck() == BST_CHECKED);
	m_pOptions->Save(VCOF_LAUNCHSTART);

	HKEY hKey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
	{
		if (m_pOptions->m_fLaunchOnStart)
		{
			CString strAppPath = TEXT("\"");
			TCHAR szBuff[MAX_PATH];
			ZeroMemory(szBuff, sizeof(szBuff));
			GetModuleFileName(NULL, szBuff, sizeof(szBuff));
			strAppPath.Append(szBuff);
			strAppPath.AppendChar('"');
			RegSetValueEx(hKey, VC_PROGRAM_NAME, 0, REG_SZ, (const BYTE*)(const TCHAR*)strAppPath, (strAppPath.GetLength() + 1)*sizeof(TCHAR));
		}
		else
		{
			RegDeleteValue(hKey, VC_PROGRAM_NAME);
		}

		RegCloseKey(hKey);
	}
}

HBRUSH CBuyVPNDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	int nCtrlId = pWnd->GetDlgCtrlID();
	if (nCtrlId == IDC_STATIC_MEMBER_AREA)
	{
		pDC->SetTextColor(RGB(0xa6, 0xed, 0x08));
		pDC->SetBkMode(TRANSPARENT);
	}
	else if (nCtrlId == IDC_STATIC_USERNAME
		|| nCtrlId == IDC_STATIC_PASSWORD
		|| nCtrlId == IDC_STATIC_STATUS_TEXT
		|| nCtrlId == IDC_STATIC_EXPIRES_TEXT
		|| nCtrlId == IDC_STATIC_BALANCE_TEXT
		|| nCtrlId == IDC_STATIC_CONFIGURATION_TEXT
		|| nCtrlId == IDC_STATIC_NETADAPTER_TEXT
		|| nCtrlId == IDC_RADIO_SINGLE_VPN
		|| nCtrlId == IDC_RADIO_DOUBLE_VPN
		|| nCtrlId == IDC_RADIO_TRIPLE_VPN
		|| nCtrlId == IDC_RADIO_QUADRO_VPN
		|| nCtrlId == IDC_RADIO_PENTA_VPN
		|| nCtrlId == IDC_STATIC_VPNCHAINS_TEXT)
	{
		pDC->SetTextColor(0xffffff);
		pDC->SetBkMode(TRANSPARENT);
	}
	else if (nCtrlId == IDC_EDIT_LOGIN
		|| nCtrlId == IDC_EDIT_PASSWORD
		|| nCtrlId == IDC_COMBO_CONFIGURATION
		|| nCtrlId == IDC_COMBO_NETWORK)
	{
		pDC->SetTextColor(0xffffff);
		pDC->SetBkColor(0x3a3a3a);
	}


	return m_brush;
}

BOOL CBuyVPNDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	int ctrlId = pWnd->GetDlgCtrlID();
	if (ctrlId == IDC_STATIC_MEMBER_AREA
		|| ctrlId == IDC_BUTTON_CONNECT
		|| ctrlId == IDC_BUTTON_EDIT
		|| ctrlId == IDC_BUTTON_SHOWLOG
		|| ctrlId == IDC_BUTTON_EXIT
		|| ctrlId == IDC_BUTTON_MIN)
	{
		SetCursor(m_curHand);
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CBuyVPNDlg::OnStnClickedStaticMemberArea()
{
	CString strUrl;
	strUrl.Format(TEXT("https://%s%s?user=%s&passwd=%s"), m_pHostsConfig->GetCurrentHost(), VC_MEMBER_AREA, m_pOptions->m_strUsername, m_pOptions->m_strPassword);

	ShellExecute(NULL, TEXT("open"), strUrl, NULL, NULL, SW_SHOW);
}


void CBuyVPNDlg::OnCbnSelchangeComboConfiguration()
{
	int iSel = m_cobConfiguration.GetCurSel();
	m_cobConfiguration.GetLBText(iSel, m_pOptions->m_strConfiguration);
	m_pOptions->Save(VCOF_CONFIGURATION);
}

void CBuyVPNDlg::OnBnClickedButtonEdit()
{
	if (m_pOptions->m_strConfiguration.IsEmpty())
	{
		return;
	}

	CString strPath = m_pConfigurations->GetPathByName(m_pOptions->m_strConfiguration);
	if (strPath.IsEmpty())
	{
		return;
	}

	ShellExecute(NULL, TEXT("open"), TEXT("notepad.exe"), strPath, NULL, SW_SHOW);
}

void CBuyVPNDlg::ChangeState(DWORD dwState)
{
	m_dwState = dwState;

	switch (dwState)
	{
	case VVC_STATE_DISCONNECTED:
		m_btnConnect.SetWindowText(TEXT("Connect"));
		m_btnConnect.EnableWindow(TRUE);
		m_stStatusText.SetWindowText(TEXT("Disconnected."));
		ModifyNotifyIcon(FALSE);
		break;
	case VVC_STATE_STARTING:
		m_btnConnect.SetWindowText(TEXT("Disconnect"));
		m_btnConnect.EnableWindow(FALSE);
		m_stStatusText.SetWindowText(TEXT("Connecting..."));
		break;
	case VVC_STATE_CONNECTING:
		m_btnConnect.SetWindowText(TEXT("Disconnect"));
		m_btnConnect.EnableWindow(TRUE);
		m_stStatusText.SetWindowText(TEXT("Connecting..."));
		break;
	case VVC_STATE_CONNECTED:
		m_btnConnect.SetWindowText(TEXT("Disconnect"));
		m_btnConnect.EnableWindow(TRUE);
		m_stStatusText.SetWindowText(TEXT("Connected."));
		ModifyNotifyIcon(TRUE);
		break;
	case VVC_STATE_DISCONNECTING:
		m_btnConnect.SetWindowText(TEXT("Connect"));
		m_btnConnect.EnableWindow(FALSE);
		m_stStatusText.SetWindowText(TEXT("Disconnecting..."));
		ModifyNotifyIcon(FALSE);
		break;
	}
}

void CBuyVPNDlg::OnBnClickedButtonShowlog()
{
	ShellExecute(NULL, TEXT("open"), TEXT("notepad.exe"), GetLogFilename(), NULL, SW_SHOW);
}

HICON CBuyVPNDlg::LoadSmIcon(DWORD dwResId)
{
	HINSTANCE hInstRes = AfxGetResourceHandle();
	int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);
	HANDLE hSmIcon = ::LoadImage(hInstRes, MAKEINTRESOURCE(dwResId), IMAGE_ICON, cxSmIcon, cySmIcon, 0);
	return (static_cast<HICON>(hSmIcon));
}

void CBuyVPNDlg::SetAccountInfo(CString strBalance, CString strExpires)
{
	m_strBalance = TEXT("Balance: ") + strBalance;
	m_strExpires = TEXT("Expires: ") + strExpires;
}

LRESULT CBuyVPNDlg::OnUpdateAccInfo(WPARAM wParam, LPARAM lParam)
{
///m_stExpiresText.SetWindowText(m_strExpires);
	m_stExpiresText.SetWindowText(L"Expires - 2018-12-30 09:17:52");
	m_stBalanceText.SetWindowText(m_strBalance);

	return TRUE;
}

void CBuyVPNDlg::CheckConfig(BOOL fUpdateAlways, CString strVersion, CString strUrl)
{
	if (strVersion.IsEmpty() || strUrl.IsEmpty())
	{
		return;
	}

	if ((fUpdateAlways || m_pOptions->m_strConfigVer.Compare(strVersion) != 0)
		&& strUrl.Find(TEXT("http")) == 0)
	{
		TCHAR szTempFilename[MAX_PATH];
		GetTempPath(MAX_PATH, szTempFilename);
		lstrcat(szTempFilename, VC_PROGRAM_NAME TEXT("_Config.zip"));
		if (CHttpWorker::LoadFile(strUrl, szTempFilename))
		{
			CString strRunningDir = GetRunningDir();
			CString strConfigDir = strRunningDir + TEXT("config");
			CString strTempDir = strConfigDir + TEXT("_old");

			MoveFile(strConfigDir, strTempDir);
			//CreateDirectory(strConfigDir, NULL);
			CString strConfigFilename = strRunningDir + TEXT("config.zip");
			MoveFile(szTempFilename, strConfigFilename);

			HZIP hz = OpenZip(strConfigFilename, 0);
			if (!hz)
			{
				DeleteFile(strConfigFilename);
				MoveFile(strTempDir, strConfigDir);
				return;
			}

			ZIPENTRY ze; 
			GetZipItem(hz, -1, &ze); 
			int numitems = ze.index;
			for (int i = 0; i < numitems; i++)
			{ 
				GetZipItem(hz, i, &ze);
				if (ze.attr & FILE_ATTRIBUTE_DIRECTORY)
				{
					CString strNewDirName = strRunningDir + ze.name;
					CreateDirectory(strNewDirName, NULL);
				}
				else
				{
					CString strNewFileName = strRunningDir + ze.name;
					UnzipItem(hz, i, strNewFileName);
				}						
			}
			CloseZip(hz);

			SHFILEOPSTRUCT fa;
			TCHAR szTempDir[MAX_PATH + 4];
			ZeroMemory(szTempDir, sizeof(szTempDir));
			lstrcpy(szTempDir, strTempDir);
			lstrcat(szTempDir, TEXT("\\\0"));
			fa.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT;
			fa.hNameMappings = NULL;
			fa.hwnd = m_hWnd;
			fa.pFrom = szTempDir;
			fa.pTo = NULL;
			fa.wFunc = FO_DELETE;
			::SHFileOperation(&fa);

			DeleteFile(strConfigFilename);

			m_pOptions->m_strConfigVer = strVersion;
			m_pOptions->Save(VCOF_CONFIGVERSION);
		}
	}

}


LRESULT CBuyVPNDlg::OnNcHitTest(CPoint point)
{
	LRESULT ret = CDialog::OnNcHitTest(point);

	if (ret ==  HTCLIENT)
		return HTCAPTION;

	return ret;
}



void CBuyVPNDlg::OnCbnSelchangeComboNetwork()
{
	int iSel = m_cobNetAdapter.GetCurSel();
	m_cobNetAdapter.GetLBText(iSel, m_pOptions->m_strNetAdapter);
	m_pOptions->Save(VCOF_NETADAPTER);
}


void CBuyVPNDlg::OnBnClickedCheckAccountState2()
{
	// TODO: Add your control notification handler code here
}


void CBuyVPNDlg::OnBnClickedRadioSingleVpn()
{
	UpdateConfigurations();
}


void CBuyVPNDlg::OnBnClickedRadioDoubleVpn()
{
	UpdateConfigurations();
}


void CBuyVPNDlg::OnBnClickedRadioTripleVpn()
{
	UpdateConfigurations();
}


void CBuyVPNDlg::OnBnClickedRadioQuadroVpn()
{
	UpdateConfigurations();
}


void CBuyVPNDlg::OnBnClickedRadioPentaVpn()
{
	UpdateConfigurations();
}

