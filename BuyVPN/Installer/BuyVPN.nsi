!define PRODUCT_NAME "InsorgVPN"
!define PRODUCT_VERSION "1.95"
!define PRODUCT_PUBLISHER "InsorgVPN"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\InsorgVPN.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define COMPANY_NAME "InsorgVPN"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ICON   "..\BuyVPN\res\BuyVPN.ico"
;!define MUI_ICON   "..\BuyVPN\res\BuyVPN.ico"
!define MUI_WELCOMEPAGE_TITLE "InsorgVPN Client installation"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------
!include WinVer.nsh

Name "${PRODUCT_NAME}"
OutFile "InsorgVPN_Install_${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

RequestExecutionLevel admin

Function .onInit
  
FunctionEnd

Section "MainSection" SEC01
  
  ;Kill process
  Processes::KillProcess "insorgvpn.exe"
  Processes::KillProcess "openvpn.exe"
 
  SetOverwrite on
  SetOutPath "$TEMP"
  File /oname=tap-windows.exe .\files\tap-windows.exe
  DetailPrint "TAP INSTALL (May need confirmation)"
  nsExec::ExecToLog '"$TEMP\tap-windows.exe" /S /SELECT_UTILITIES=1'
  Pop $R0 # return value/error/timeout
  Delete "$TEMP\tap-windows.exe"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "tap" "installed"

  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File ..\release\InsorgVPN.exe
  File /nonfatal .\files\openvpn.exe
  File /nonfatal .\files\libcrypto-1_1.dll
  File /nonfatal .\files\liblzo2-2.dll
  File /nonfatal .\files\libpkcs11-helper-1.dll
  File /nonfatal .\files\libssl-1_1.dll
  File /nonfatal /r .\files\config
  File /nonfatal /r .\files\log


  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\InsorgVPN.exe"
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\InsorgVPN.exe"
  ${If} ${AtLeastWinVista}
  ShellLink::SetRunAsAdministrator "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
  ShellLink::SetRunAsAdministrator "$DESKTOP\${PRODUCT_NAME}.lnk"
  ${EndIf}
  SetAutoClose true
SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall ${PRODUCT_NAME}.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\InsorgVPN.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\InsorgVPN.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "${PRODUCT_NAME} successfully uninstalled."
FunctionEnd

Function un.onInit
FunctionEnd

Section Uninstall

;  !include "uninstall.nsh"

;Kill process
  Processes::KillProcess "insorgvpn"
  Processes::KillProcess "openvpn"

  ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "tap"
  ${If} $R0 == "installed"
	ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\TAP-Windows" "UninstallString"
	${If} $R0 != ""
		DetailPrint "TAP UNINSTALL"
		nsExec::ExecToLog '"$R0" /S'
		Pop $R0 # return value/error/timeout
	${EndIf}
  ${EndIf}

  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall ${PRODUCT_NAME}.lnk"
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

  RMDir /r "$INSTDIR\config"
  RMDir /r "$INSTDIR\log"
  RMDir /r "$INSTDIR"
  RMDir /REBOOTOK "$INSTDIR"

  DeleteRegKey HKCU "SOFTWARE\InsorgVPN"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd