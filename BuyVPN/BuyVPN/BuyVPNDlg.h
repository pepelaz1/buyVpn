// BuyVPNDlg.h : header file
//

#pragma once

#include "resource.h"
#include "Configurations.h"
#include "NetAdapters.h"
#include "HostsConfig.h"
#include "Options.h"
#include "RunThread.h"
#include "BtnST.h"
#include "ColorCheck.h"
#include "afxwin.h"
#include "LanguageManager.h"


#define VVC_STATE_DISCONNECTED		0x00000001
#define VVC_STATE_STARTING			0x00000002
#define VVC_STATE_CONNECTING		0x00000003
#define VVC_STATE_CONNECTED			0x00000004
#define VVC_STATE_DISCONNECTING		0x00000005

#define WM_NOTIFYICON (WM_USER + 1)
#define WM_CHANGESTATE (WM_USER + 2)
#define WM_UPDATEACCINFO (WM_USER + 3)
#define WM_DISABLENONE (WM_USER + 4)


// CBuyVPNDlg dialog
class CBuyVPNDlg : public CDialog
{
// Construction
public:
	CBuyVPNDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BUYVPN_DIALOG };

	void SetAccountInfo(CString strBalance, CString strExpires);
	CLanguageManager *getLangManager();
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonMin();
	afx_msg void OnEnChangeEditLogin();
	afx_msg void OnEnChangeEditPassword();
	afx_msg void OnBnClickedCheckSavelogin();
	afx_msg void OnBnClickedCheckAccountState();
	afx_msg void OnBnClickedCheckLaunchOnstart();
	afx_msg void OnStnClickedStaticMemberArea();
	afx_msg void OnCbnSelchangeComboConfiguration();
	afx_msg void OnCbnSelchangeComboNetwork();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonShowlog();
	afx_msg void OnBnClickedCheckAccountState2();
	afx_msg void OnBnClickedRadioSingleVpn();
	afx_msg void OnBnClickedRadioDoubleVpn();
	afx_msg void OnBnClickedRadioTripleVpn();
	afx_msg void OnBnClickedRadioQuadroVpn();
	afx_msg void OnBnClickedRadioPentaVpn();
	afx_msg void OnBnClickedButtonEnglish();
	afx_msg void OnBnClickedButtonRussian();

	void AddNotifyIcon();
	void ModifyNotifyIcon(BOOL fConnected);
	void DeleteNotifyIcon();
	LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeState(WPARAM wParam, LPARAM lParam);
	LRESULT OnDisableNone(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateAccInfo(WPARAM wParam, LPARAM lParam);
	void ApplyOptions();
	void ChangeState(DWORD dwState);
	HICON LoadSmIcon(DWORD dwResId);
	void CheckConfig(BOOL fUpdateAlways, CString strVersion, CString strUrl);
	void UpdateConfigurations();
	void UpdateUITexts();


	HICON m_hIcon;
	BOOL m_fNeedClose;
	CMenu m_TrayMenu;
	CHostsConfig* m_pHostsConfig;
	COptions* m_pOptions;
	CEdit m_edUsername;
	CEdit m_edPassword;
	CColorCheck m_chbSavePass;
	CComboBox m_cobConfiguration;
	CComboBox m_cobNetAdapter;
	CColorCheck m_chbCheckAccount;
	CColorCheck m_chbCheckAccount2;
	CColorCheck m_chbCheckAccount3;

	CColorCheck m_chbLaunchOnStart;
	CStatic m_stMemberArea;
	HCURSOR m_curHand;
	CFont* m_pMembersAreaFont;
	CConfigurations* m_pConfigurations;
	CNetAdapters* m_pNetAdapters;
	CRunThread* m_pRunThread;
	DWORD m_dwState;
	CButtonST m_btnConnect;
	CButtonST m_btnEdit;
	CButtonST m_btnShowLog;
	CString m_strBalance;
	CString m_strExpires;
	HICON m_hConnected;
	HICON m_hDisconnected;
	CBrush m_brush;
	CStatic m_stLogo;
	CButtonST m_btnExit;
	CButtonST m_btnMin;
	CStatic m_stStatusText;
	CStatic m_stBalanceText;
	CStatic m_stExpiresText;
	CButton m_rbSingleVpn;

	CButton m_rbDoubleVpn;
	CButton m_rbTripleVpn;
	CButton m_rbQuadroVpn;
	CButton m_rbPentaVpn;
	CButtonST m_btnEng;
	CButtonST m_btnRus;

	CLanguageManager m_langManager;
public:
	CStatic m_stUsername;
	CStatic m_stPassword;
	CStatic m_stSelectConfiguration;
	CStatic m_stNetAdapter;
	CStatic m_stVpnChains;
};
