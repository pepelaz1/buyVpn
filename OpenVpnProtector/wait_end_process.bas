Attribute VB_Name = "process"


Public Function detect_start_process(proc As String, adapt As String, res_exit As String) As String
 'proc - процесс,запуск которого проверяем
 'adapt - часть описания сетевого адаптера,который проверяем и который зависет от proc
 'по какому результату выходим из функции(что детектим - старт или стоп процесса)
 'Option Explicit

 Dim objSWbemServicesEx
 Dim collSWbemObjectSet_Win32
 Dim objSWbemObjectEx_Win32

 Dim strComputer: strComputer = "."
 Dim elem
 Dim flag_adapt: flag_adapt = 0
 Dim Count: Count = 0
 Dim result As String: result = ""
 
 Set objSWbemServicesEx = GetObject _
    ("winmgmts:{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
 
Do
 
  'ищем запущенный процесс
  Set collSWbemObjectSet_Win32 = objSWbemServicesEx.ExecQuery _
     ("SELECT * FROM Win32_Process WHERE Caption ='" & proc & "'")

  Count = 0

  For Each objSWbemObjectEx_Win32 In collSWbemObjectSet_Win32
   With objSWbemObjectEx_Win32
    Count = Count + 1
   End With
  Next
   
  'определяем включен ли сетевой адаптер
  'flag_adapt = 0
  'Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\CIMV2")
  Set colItems = objSWbemServicesEx.ExecQuery( _
    "SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = 'True'", , 48)
  
  flag_adapt = 0
  For Each objItem In colItems
   If InStr(objItem.Description, adapt) > 0 And Not IsNull(objItem.IPAddress) Then
     For Each elem In objItem.IPAddress
      If InStr(elem, "0.0.0.0") > 0 Then
       flag_adapt = 0
      Else
       flag_adapt = 1
       Exit For
      End If
     Next
   End If
  Next

  'DoEvents
  If flag_adapt = 1 And Count > 0 Then
   'включено
   result = "start"
  Else
   If flag_adapt = 0 And Count = 0 Then
    'выключено
    result = "stop"
   Else
    result = ""
   End If
  End If
  
 'Set collSWbemObjectSet_Win32 = Nothing
 'Set colItems = Nothing
 Call Sleep(400)
Loop Until result = res_exit
 'MsgBox "result: " & result & " res_exit: " & res_exit & " flag_adapt: " & flag_adapt & " Count: " & Count




Set collSWbemObjectSet_Win32 = Nothing
Set objSWbemServicesEx = Nothing
'Set objWMIService = Nothing
Set colItems = Nothing

detect_start_process = result
          
End Function
