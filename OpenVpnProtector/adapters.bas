Attribute VB_Name = "adapters"
Private Function setgateway(ip_gateway As String, ip_metric As String, ip_adapter As String) As Integer
 'Option Explicit
 'Option Compare Text


Dim objSWbemServicesEx
Dim collSWbemObjectSet_Win32_NetworkAdapterConfiguration
Dim objSWbemObjectEx_Win32_NetworkAdapterConfiguration

Dim strComputer
Dim elem
Dim strGateway: strGateway = Array(ip_gateway)
Dim strGatewayMetric: strGatewayMetric = Array(Val(ip_metric))
Dim TestComp As Integer
strComputer = "."
Dim elem2 As String

Set objSWbemServicesEx = GetObject _
    ("winmgmts:{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set collSWbemObjectSet_Win32_NetworkAdapterConfiguration = objSWbemServicesEx.ExecQuery _
    ("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = 'True'")

For Each objSWbemObjectEx_Win32_NetworkAdapterConfiguration In collSWbemObjectSet_Win32_NetworkAdapterConfiguration
    With objSWbemObjectEx_Win32_NetworkAdapterConfiguration
        
           
           If Not IsNull(.IPAddress) Then
            For Each elem In .IPAddress
             'MsgBox "Просматриваемый IP адрес." & elem & ".Где поменять." & ip_adapter & "."
               'проверяем тот ли адаптер и чтобы он не был WAN
               If elem Like ip_adapter And InStr(.Description, "WAN") = 0 Then
                   'If elem = ip_adapter Then
                   ' MsgBox "Меняем для " & ip_adapter & "гетевей на " & strGateway(0) & "метрику на " & strGatewayMetric(0)
                    setgateway = objSWbemObjectEx_Win32_NetworkAdapterConfiguration.SetGateways(strGateway, strGatewayMetric)
             End If
            Next
           End If

    End With
Next

Set collSWbemObjectSet_Win32_NetworkAdapterConfiguration = Nothing
Set objSWbemServicesEx = Nothing

'WScript.Quit 0
End Function

Private Function search_ID_adapter(name_adapt As String) As String
 strComputer = "."
 Set objWMIService = GetObject("winmgmts:\\" _
    & strComputer & "\root\CIMV2")
 Set colItems = objWMIService.ExecQuery( _
    "SELECT * FROM Win32_NetworkAdapterConfiguration", , 48)
 For Each objItem In colItems
  If InStr(objItem.Description, name_adapt) > 0 Then
   search_ID_adapter = objItem.SettingID
  End If
 Next
 Set objWMIService = Nothing
 Set colItems = Nothing
End Function

Public Sub set_bind_tap()
 Const HKLM = &H80000002
 sComputer = "."
 Set oReg = GetObject("winmgmts:{impersonationLevel=impersonate}!\\" _
      & sComputer & "\root\default:StdRegProv")

 sKeyPath = "SYSTEM\CurrentControlSet\Services\Tcpip\Linkage"
 sValueName = "Bind"

 oReg.GetMultiStringValue HKLM, sKeyPath, sValueName, arValues

 'arValuesNew = Array()
 Dim arValuesNew() As Variant
 not_empty_arValuesNew = 1
 
 id_tap = search_ID_adapter("TAP-")
 
' MsgBox "id_tap:  " & id_tap

  For i = 0 To UBound(arValues)
   If i = 0 Then
     If arValues(i) = "\Device\" & id_tap Then
       ' entry is alredy first in the list, no point in continuing
       not_empty_arValuesNew = 0
      ' MsgBox "not_empty_arValuesNew:  " & not_empty_arValuesNew
       Exit For
     Else
       ' put NdisWanIp in the first element in the new array
       ReDim Preserve arValuesNew(0)
       arValuesNew(0) = "\Device\" & id_tap
     End If
   End If

   If arValues(i) <> "\Device\" & id_tap Then
     iCountNew = UBound(arValuesNew) + 1
     ReDim Preserve arValuesNew(iCountNew)
     arValuesNew(iCountNew) = arValues(i)
   End If
  Next

  If not_empty_arValuesNew Then
   oReg.SetMultiStringValue HKLM, sKeyPath, sValueName, arValuesNew
  End If
  Set oReg = Nothing
End Sub
