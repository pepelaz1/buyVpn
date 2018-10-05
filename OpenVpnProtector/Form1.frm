VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "OpenVPN Protector"
   ClientHeight    =   1650
   ClientLeft      =   165
   ClientTop       =   810
   ClientWidth     =   3495
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   1650
   ScaleWidth      =   3495
   StartUpPosition =   3  'Windows Default
   Visible         =   0   'False
   Begin VB.PictureBox icon2 
      Height          =   375
      Left            =   120
      Picture         =   "Form1.frx":0682
      ScaleHeight     =   315
      ScaleWidth      =   315
      TabIndex        =   1
      Top             =   840
      Width           =   375
   End
   Begin VB.PictureBox icon1 
      Height          =   375
      Left            =   0
      Picture         =   "Form1.frx":0A0C
      ScaleHeight     =   315
      ScaleWidth      =   315
      TabIndex        =   0
      Top             =   360
      Width           =   375
   End
   Begin VB.Menu menumain 
      Caption         =   "main"
      Begin VB.Menu menuexit 
         Caption         =   "exit"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'для иконки
Private Declare Function Shell_NotifyIcon Lib "shell32.dll" Alias "Shell_NotifyIconA" (ByVal dwMessage As Long, lpData As TNotifyIconData) As Long
Private Const NIM_ADD = &H0
Private Const NIM_MODIFY = &H1
Private Const NIM_DELETE = &H2
Private Const NIF_MESSAGE = &H1
Private Const NIF_ICON = &H2
Private Const NIF_TIP = &H4
Private Const WM_MOUSEMOVE = &H200
Private Const WM_LBUTTONDOWN = &H201
Private Const WM_LBUTTONUP = &H202
Private Const WM_LBUTTONDBLCLK = &H203
Private Const WM_RBUTTONDOWN = &H204
Private Const WM_RBUTTONUP = &H205
Private Const WM_RBUTTONDBLCLK = &H206
Private Const WM_MBUTTONDOWN = &H207
Private Const WM_MBUTTONUP = &H208
Private Const WM_MBUTTONDBLCLK = &H209

Private Type TNotifyIconData
cbSize As Long
hWnd As Long
uId As Long
uFlags As Long
ucallbackMessage As Long
hIcon As Long
szTip As String * 64
End Type
Dim Nid As TNotifyIconData
' *** Tray Icon ***
Private Function TrayAddIcon_start(ByVal mForm As Form) As TNotifyIconData
TrayAddIcon_start.cbSize = Len(Nid)
TrayAddIcon_start.hIcon = mForm.icon1
TrayAddIcon_start.hWnd = mForm.hWnd
TrayAddIcon_start.szTip = mForm.Caption & vbNullChar
TrayAddIcon_start.ucallbackMessage = 512
TrayAddIcon_start.uFlags = NIF_MESSAGE Or NIF_ICON Or NIF_TIP
TrayAddIcon_start.uId = 1
Call Shell_NotifyIcon(NIM_ADD, TrayAddIcon_start)
End Function
Private Function TrayAddIcon_stop(ByVal mForm As Form) As TNotifyIconData
TrayAddIcon_stop.cbSize = Len(Nid)
TrayAddIcon_stop.hIcon = mForm.icon2
TrayAddIcon_stop.hWnd = mForm.hWnd
TrayAddIcon_stop.szTip = mForm.Caption & vbNullChar
TrayAddIcon_stop.ucallbackMessage = 512
TrayAddIcon_stop.uFlags = NIF_MESSAGE Or NIF_ICON Or NIF_TIP
TrayAddIcon_stop.uId = 1
Call Shell_NotifyIcon(NIM_ADD, TrayAddIcon_stop)
End Function
Private Sub TrayRemoveIcon(IconData As TNotifyIconData)
Call Shell_NotifyIcon(NIM_DELETE, IconData)
End Sub
Private Sub TrayModifyIcon(IconData As TNotifyIconData)
Call Shell_NotifyIcon(NIM_MODIFY, IconData)
End Sub



Private Sub Form_Load()
 Dim Array_0() As String
 Dim Array_0_check() As String
 Dim Array_10() As String
 index_wait_end_process = 0
 Dim vhodit As Boolean: vhodit = False

 osVersVista = 0
  If system_info.VersionWindows("") = "Windows Server 2008" Or system_info.VersionWindows("") = "Vista" Then
   osVersVista = 1
  End If
  
'определяем язык системы
notRus = 0
If InStr(system_info.GetSystemLanguage(LangID), "419") = 0 Then
 notRus = 1
End If

'MsgBox "Vista:  " & osVersVista & "   Not Russian: " & notRus
'MsgBox "TestTestTestTestTestTestTestTestTestTest", 16 + vbSystemModal, "OpenVPN Protector"

'проверяем защиту проги
'If protect.protect_prog = 1 Then
'If notRus Then
'    MsgBox vbCrLf & "The code of the program has been changed without permission!   " _
'     & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
'     & vbCrLf & "support@safe-inet.com" _
'     & vbCrLf & "ICQ:802034", 16 + vbSystemModal, "OpenVPN Protector"
'   Else
'    MsgBox vbCrLf & "Код утилиты несанкционированно изменен!   " _
'     & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
'     & vbCrLf & "support@safe-inet.com" _
'     & vbCrLf & "ICQ:802034", 16 + vbSystemModal, "OpenVPN Protector"
'   End If
'  End
' End If

'проверяем, запущена ли программа
 
  If App.PrevInstance = True Then
   If notRus Then
    MsgBox vbCrLf & "The OpenVPN Protector is already running!   " _
     & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
     & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
   Else
    MsgBox vbCrLf & "OpenVPN Protector уже работает!   " _
     & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
     & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
   End If
  End
 End If
 
Do
 index_wait_end_process = 0
 'запускаем иконку
 'Call TrayRemoveIcon(Nid)
 Nid = TrayAddIcon_stop(Me)
 
 'определяем к-во маршрутов 0.0.0.0 перед подключением процесса
 'Array_0 = detect_0_route.detect_0
 

 
 'запускаем процесс отслеживания запуска процесса
 res_proc = process.detect_start_process("openvpn.exe", "TAP-", "start")
 
 Call Sleep(3000)
 
If osVersVista Then
  Call Sleep(3000)
End If

  'ищем гетевеи в системе
Dim system_gateway() As String
system_gateway = detect_default_gateway.search_gateway
Call Sleep(300)

'дебуг
'If system_gateway(0, 0) <> "" Then
' For l1 = 0 To UBound(system_gateway)
'  For l2 = 0 To UBound(system_gateway, 2)
'   If l1 = 0 Then
'    MsgBox "system_gateway(" & l2 & ")" & system_gateway(0, l2)
'   Else
'    MsgBox "DHCPEnabled(" & l2 & ")" & system_gateway(1, l2)
'   End If
'  Next
' Next
'Else
' MsgBox "system_gateway: Null"
'End If

 'ищем индекс маршрута по умолчанию. если есть маршрут 0.0.0.0 128.0.0.0,то ищем его,иначе
 '0.0.0.0 0.0.0.0 с наименьшей метрикой
Array_0 = detect_route.route_array("0.0.0.0")
Array_10 = detect_route.route_array("10.0.0.0")

'дебуг
'  For i = 0 To UBound(Array_0, 2)
'    MsgBox (Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i) & " " & Array_0(4, i))
'  Next

  If Array_0(0, 0) = "" Then
   If notRus Then
    MsgBox vbCrLf & "No connection to the Internet!   " _
     & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
     & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
   Else
    MsgBox vbCrLf & "Нет соединения с Интернет!   " _
     & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
     & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
   End If
   'Call TrayRemoveIcon(Nid)
   'Nid = TrayAddIcon_stop(Me)
   index_wait_end_process = 1
   GoTo wait_end_process
  End If
  
low_metric_128 = 999999
gateway_index = UBound(Array_0, 2)
  
  For i = 0 To UBound(Array_0, 2)
   If InStr(Array_0(UBound(Array_0) - 3, i), "128.0.0.0") <> 0 Then
    'Else
    If Val(Array_0(UBound(Array_0), i)) < low_metric_128 Then
     low_metric_128 = Val(Array_0(UBound(Array_0), i))
     gateway_index = i
     End If
    End If
  Next
  
'  MsgBox "low_metric_128: " & low_metric_128
'  MsgBox "gateway_index: " & gateway_index
 
  If UBound(Array_0, 2) = 0 Then
   If notRus Then
    MsgBox vbCrLf & "Only one connection is used!   " _
     & vbCrLf & "OpenVPN protection is not enabled!   " _
     & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
     & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
   Else
    MsgBox vbCrLf & "Только одно подключение используется!   " _
     & vbCrLf & "Защита OpenVPN не включена!   " _
     & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
     & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
   End If
   'Nid = TrayAddIcon_stop(Me)
   index_wait_end_process = 1
   GoTo wait_end_process
  End If
 'MsgBox "low_metric_index" & low_metric_index
 
 
 Set objShell = CreateObject("WScript.Shell")
  
 'удаляем все нулевые маршруты,кроме маршрута OpenVPN 0.0.0.0 mask 128.0.0.0 для висты
 'эта команда удалит все маршруты 0.0.0.0 в XP
 Call Sleep(500)
 objShell.Run "cmd /C route delete 0.0.0.0 mask 0.0.0.0", 0, True
 'удаляем маршрут 10.0.0.0 для избежания конфликта с впн ДНС
  If Array_10(0, 0) <> "" Then
   'MsgBox "system_gateway: Удаляем 10.0.0.0"
   objShell.Run "cmd /C route delete 10.0.0.0", 0, True
  End If
 
  Call Sleep(200)
 'добавляем маршрут с наименьшей метрикой в XP
 If osVersVista = 0 Then
   For i = 0 To UBound(Array_0, 2)
   If i = gateway_index Then
    'MsgBox ("cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i))
    objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i), 0, True
   End If
  Next
End If
   
Call Sleep(400)
' MsgBox "ставим bind tap адаптера на первую строчку реестра"
 adapters.set_bind_tap

  'чекаем количество нулевых маршрутов и если их более одного,то защита не включилась
  Array_0_check = detect_route.route_array("0.0.0.0")
  'если один маршрут,то загружаем зеленую иконку
  If UBound(Array_0_check, 2) = 0 And Array_0_check(0, 0) <> "" Then
   Call Sleep(400)
   'загружаем иконку в трей
   Call TrayRemoveIcon(Nid)
   Nid = TrayAddIcon_start(Me)
  Else
   index_wait_end_process = 1
   If osVersVista Then
    If notRus Then
     MsgBox vbCrLf & "Incorrect routing table!   " _
      & vbCrLf & "Perhaps the utility is not running on behalf of the Administrator.   " _
      & vbCrLf & "OpenVPN protection is not enabled!   " _
      & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
      & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
    Else
     MsgBox vbCrLf & "Некорректная таблица маршрутизации!   " _
      & vbCrLf & "Вероятно утилита запущена не от имени Администратора.   " _
      & vbCrLf & "Защита OpenVPN не включена!   " _
      & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
      & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
    End If
   Else
    If notRus Then
     MsgBox vbCrLf & "Incorrect routing table!   " _
      & vbCrLf & "OpenVPN protection is not enabled!   " _
      & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
      & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
    Else
     MsgBox vbCrLf & "Некорректная таблица маршрутизации!   " _
      & vbCrLf & "Защита OpenVPN не включена!   " _
      & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
      & vbCrLf & "http://safe-inet.com", 16 + vbSystemModal, "OpenVPN Protector"
    End If
   End If
  End If

wait_end_process:
 
'запускаем процесс отслеживания остановки процесса
 res_proc = process.detect_start_process("openvpn.exe", "TAP-", "stop")
  
 'если не было маршрутов или был только один или другие ошибки,то нечего добавлять, пропускаем
 If index_wait_end_process = 1 Then GoTo pass_and_start
  
 If notRus Then
   response = MsgBox(vbCrLf & "The OpenVPN is disabled!   " & vbCrLf & "Do you want to connect to the Internet?   " _
    & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
    & vbCrLf & "http://safe-inet.com", vbYesNo + vbQuestion + vbDefaultButton1 + vbSystemModal, "OpenVPN Protector")
 Else
   response = MsgBox(vbCrLf & "OpenVPN отключен! Включить Интернет?   " _
    & vbCrLf & vbCrLf & "http://vpn.insorg.org" _
    & vbCrLf & "http://safe-inet.com", vbYesNo + vbQuestion + vbDefaultButton2 + vbSystemModal, "OpenVPN Protector")
 End If
 If response = vbNo Then GoTo pass_and_start
 
'возвращаем маршруты 0.0.0.0 в последовательности увеличения приоритета

 If osVersVista Then
  'возвращаем все defaultGateways если они были до подключения для Висты
  'MsgBox system_gateway(0, 0) & " Возвращаем постоянные гетевеи"
  If system_gateway(0, 0) <> "" Then
   'MsgBox "system_gateway(0, 0) <> Возвращаем постоянные гетевеи"
   For i = 0 To UBound(Array_0, 2)
    If InStr(Array_0(UBound(Array_0) - 2, i), ".") = 0 Then
      For l2 = 0 To UBound(system_gateway, 2)
       'MsgBox "system_gateway(" & l & ")" & system_gateway(l)
        If system_gateway(0, l2) = Array_0(3, i) Then
          If system_gateway(1, l2) = "False" Then
           objShell.Run "cmd /C route -p add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i), 0, True
           'MsgBox ("1 cmd /C route -p add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i))
          Else
           objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i), 0, True
           'MsgBox ("2 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i))
           'objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i) & " METRIC " & Array_0(4, i), 0, True
           'MsgBox ("2 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i) & " METRIC " & Array_0(4, i))
          End If
        End If
      Next
    Else
     For l2 = 0 To UBound(system_gateway, 2)
       'MsgBox "system_gateway(" & l & ")" & system_gateway(l)
        If system_gateway(0, l2) = Array_0(2, i) Then
          If system_gateway(1, l2) = "False" Then
           objShell.Run "cmd /C route -p add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i), 0, True
           'MsgBox ("3 cmd /C route -p add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i))
          Else
           objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i), 0, True
           'MsgBox ("4 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i))
           'objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i) & " METRIC " & Array_0(4, i), 0, True
           'MsgBox ("4 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i) & " METRIC " & Array_0(4, i))
          End If
        End If
      Next
    End If
   Call Sleep(200)
   Next
  End If
   
   'возвращаем другой временный маршрут,например Wan, в том случае если он был единственный до подключения
   'и был ответ "Да" на вопрос о подключении Интернета и если он не равен Gateway
       
   'закомментировав след строку мы возвращаем все временные маршруты
     For i = 0 To UBound(Array_0, 2)
      If InStr(Array_0(UBound(Array_0) - 2, i), ".") = 0 Then
       If i <> gateway_index Then
        For l2 = 0 To UBound(system_gateway, 2)
         If Array_0(3, i) = system_gateway(0, l2) Then
          vhodit = True
          'MsgBox "1 vhodit = True"
          Exit For
         End If
        Next
        If vhodit = False Then
         objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i), 0, True
         'MsgBox ("5 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i))
         'objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i) & " METRIC " & Array_0(4, i), 0, True
         'MsgBox ("5 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i) & " METRIC " & Array_0(4, i))
        End If
       End If
      Else
       If i <> gateway_index Then
        For l2 = 0 To UBound(system_gateway, 2)
         If Array_0(2, i) = system_gateway(0, l2) Then
          vhodit = True
          'MsgBox "2 vhodit = True"
          Exit For
         End If
        Next
        If vhodit = False Then
         objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i), 0, True
         'MsgBox ("6 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i))
         'objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i) & " METRIC " & Array_0(4, i), 0, True
         'MsgBox ("6 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i) & " METRIC " & Array_0(4, i))
        End If
       End If
      End If
      vhodit = False
      Call Sleep(200)
     Next
   
  Else
   'Для XP и т.д.
   For i = 0 To UBound(Array_0, 2)
    If i <> gateway_index Then
     'If InStr(Array_0(UBound(Array_10) - 2, i), ".") = 0 Then
     '  objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i), 0, True
     '  MsgBox ("XP and Vista/7 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(3, i))
       'objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i) & " METRIC " & Array_0(4, i), 0, True
     '  'MsgBox ("XP cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i) & " METRIC " & Array_0(4, i))
     'Else
       'objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i), 0, True
       objShell.Run "cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i) & " METRIC " & Array_0(4, i), 0, True
       'MsgBox ("XP and Vista/7 cmd /C route add " & Array_0(0, i) & " mask " & Array_0(1, i) & " " & Array_0(2, i))
     'End If
    End If
    Call Sleep(200)
   Next
 End If
 
 'возвращаем маршрут 10.0.0.0
 If Array_10(0, 0) <> "" Then
   'MsgBox "Возвращаем 10.0.0.0"
   For i = 0 To UBound(Array_10, 2)
    If InStr(Array_10(UBound(Array_10) - 2, i), ".") = 0 Then
      objShell.Run "cmd /C route add " & Array_10(0, i) & " mask " & Array_10(1, i) & " " & Array_10(3, i), 0, True
      'MsgBox ("cmd /C route add " & Array_10(0, i) & " mask " & Array_10(1, i) & " " & Array_10(3, i))
      'objShell.Run "cmd /C route add " & Array_10(0, i) & " mask " & Array_10(1, i) & " " & Array_10(3, i) & " METRIC " & Array_10(4, i), 0, True
      'MsgBox ("cmd /C route add " & Array_10(0, i) & " mask " & Array_10(1, i) & " " & Array_10(3, i) & " METRIC " & Array_10(4, i))
      Call Sleep(200)
    Else
      objShell.Run "cmd /C route add " & Array_10(0, i) & " mask " & Array_10(1, i) & " " & Array_10(2, i), 0, True
      'MsgBox ("cmd /C route add " & Array_10(0, i) & " mask " & Array_10(1, i) & " " & Array_10(2, i))
      'objShell.Run "cmd /C route add " & Array_10(0, i) & " mask " & Array_10(1, i) & " " & Array_10(2, i) & " METRIC " & Array_10(4, i), 0, True
      'MsgBox ("cmd /C route add " & Array_10(0, i) & " mask " & Array_10(1, i) & " " & Array_10(2, i) & " METRIC " & Array_10(4, i))
      Call Sleep(200)
    End If
   Next
 End If
pass_and_start:
 'убираем иконку с трея
 Call TrayRemoveIcon(Nid)
Loop Until False
 Call Sleep(200)
 'убираем иконку с трея
 Call TrayRemoveIcon(Nid)
 End
End Sub

Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
   Dim Message As Long
   Message = X / Screen.TwipsPerPixelX

   Select Case Message
     Case WM_RBUTTONDOWN
       PopupMenu menumain
   End Select
End Sub

Private Sub Form_Unload(Cancel As Integer)
Call TrayRemoveIcon(Nid)
End Sub

Private Sub menuexit_Click()
  Close
  Unload Me
  End
End Sub
