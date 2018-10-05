Attribute VB_Name = "system_info"
Private Declare Function GetVersionExA Lib "kernel32" (lpVersionInformation As OSVERSIONINFOEX) As Integer
 
Private Const VER_PLATFORM_WIN32_WINDOWS = 1
Private Const VER_PLATFORM_WIN32_NT = 2

'wProductType
Private Const VER_NT_WORKSTATION = 1 'Windows Vista, Windows XP Professional, Windows XP Home Edition, or Windows 2000 Professional
Private Const VER_NT_DOMAIN_CONTROLLER = 2 'Controleur de domaine sous Windows Serveur 2008,2003 ou 2000
Private Const VER_NT_SERVER = 3 'Windows Serveur 2008 , 2003 or 2000


Private Type OSVERSIONINFOEX
   dwOSVersionInfoSize As Long
   dwMajorVersion As Long
   dwMinorVersion As Long
   dwBuildNumber As Long
   dwPlatformId As Long
   szCSDVersion As String * 128
   wServicePackMajor As Integer
   wServicePackMinor As Integer
   wSuiteMask As Integer
   wProductType As Byte
   wReserved As Byte
End Type

Private Declare Function GetSystemDefaultLangID Lib "kernel32" () As Long
Private Declare Function VerLanguageName Lib "kernel32" Alias "VerLanguageNameA" _
(ByVal wLang As Long, ByVal szLang As String, ByVal nSize As Long) As Long

Public Enum pLang
    LangID = 1
    LangName = 2
End Enum


Public Function GetSystemLanguage(param As pLang) As String
    Dim ID As String
    Dim Buffer As String
    ID = "&H" & Right(Hex(GetSystemDefaultLangID()), 3)
    Select Case param
    Case 1
        GetSystemLanguage = ID & " / " & CStr(Val(ID))
    Case 2
        Buffer = String(255, 0)
        VerLanguageName CLng(ID), Buffer, Len(Buffer)
        Buffer = Left$(Buffer, InStr(1, Buffer, Chr$(0)) - 1)
        GetSystemLanguage = Buffer
    End Select
End Function

 
Public Function VersionWindows(ByRef sp As String) As String

    Dim os As OSVERSIONINFOEX
           
    os.dwOSVersionInfoSize = Len(os)
    GetVersionExA os
    sp = ""
   
    With os
        Select Case .dwPlatformId
            Case VER_PLATFORM_WIN32_WINDOWS
                Select Case .dwMinorVersion
                    Case 0
                        VersionWindows = "95"
                    Case 10
                        VersionWindows = "98"
                    Case 90
                        VersionWindows = "Me"
                End Select
            Case VER_PLATFORM_WIN32_NT
                Select Case .dwMajorVersion
                    Case 3
                        VersionWindows = "NT 3.51"
                    Case 4
                        VersionWindows = "NT 4.0"
                    Case 5
                        If .dwMinorVersion = 0 Then
                            VersionWindows = "2000"
                        Else
                            VersionWindows = "XP"
                        End If
                    Case 6
                        If .wProductType = VER_NT_WORKSTATION Then
                             VersionWindows = "Vista"
                        Else
                            VersionWindows = "Windows Server 2008"
                        End If
                        
                        
                End Select
        End Select
       
        If InStr(.szCSDVersion, Chr(0)) > 0 Then
            sp = Left(.szCSDVersion, InStr(.szCSDVersion, Chr(0)) - 1)
        End If
    End With
   
End Function


