Attribute VB_Name = "protect"
Option Explicit
Private Declare Function CreateMutex Lib "kernel32" Alias "CreateMutexA" (lpMutexAttributes As SECURITY_ATTRIBUTES, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
Private Declare Function GetLastError& Lib "kernel32" ()
Private Const ERROR_ALREADY_EXISTS = 183&
Private Type SECURITY_ATTRIBUTES
        nLength As Long
        lpSecurityDescriptor As Long
        bInheritHandle As Long
End Type

    Dim hMutex As Long


Function protect_prog() As Integer
'эта защита для OriEN. возвращает 1 если защита взломана иначе 0
Dim sa As SECURITY_ATTRIBUTES
sa.bInheritHandle = 1
sa.lpSecurityDescriptor = 0
sa.nLength = Len(sa)
    
    hMutex = CreateMutex(sa, False, "xBIqt70uxdq65Dm")
    If (Err.LastDllError <> ERROR_ALREADY_EXISTS) Then
     protect_prog = 1
    Else
     protect_prog = 0
    End If
End Function
