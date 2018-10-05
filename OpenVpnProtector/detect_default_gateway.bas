Attribute VB_Name = "detect_default_gateway"
Public Function search_gateway() As String()

Dim objSWbemServicesEx
Dim collSWbemObjectSet_Win32_NetworkAdapterConfiguration
Dim objSWbemObjectEx_Win32_NetworkAdapterConfiguration

Dim strComputer
Dim elem
Dim strGateway() As String ': strGateway = ""
'Dim strGatewayMetric: strGatewayMetric = Array(Val(ip_metric))
Dim isch_get As Integer: isch_get = -1
Dim een As Integer: een = 0
strComputer = "."

Set objSWbemServicesEx = GetObject _
    ("winmgmts:{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set collSWbemObjectSet_Win32_NetworkAdapterConfiguration = objSWbemServicesEx.ExecQuery _
    ("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = 'True'")

For Each objSWbemObjectEx_Win32_NetworkAdapterConfiguration In collSWbemObjectSet_Win32_NetworkAdapterConfiguration
    With objSWbemObjectEx_Win32_NetworkAdapterConfiguration
        'isch_get - максимальное занчение массива в который вносим все гетевеи
        'isch_get=-1 начальное значение
        'isch_get=isch_get+UBound(objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DefaultIPGateway)+1
        'objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DefaultIPGateway - массив гетевеев для конкретного адаптера
        If Not IsNull(objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DefaultIPGateway) Then
          isch_get = 1 + isch_get + UBound(objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DefaultIPGateway)
          ReDim Preserve strGateway(1, isch_get)
         For ii = 0 To UBound(objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DefaultIPGateway)
          'MsgBox "strGateway(0, " & (een + ii) & ") " & objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DefaultIPGateway(ii)
          'MsgBox "strGateway(1, " & (een + ii) & ") " & objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DHCPEnabled
          strGateway(0, een + ii) = objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DefaultIPGateway(ii)
          strGateway(1, een + ii) = objSWbemObjectEx_Win32_NetworkAdapterConfiguration.DHCPEnabled
         Next
          een = 1 + isch_get
          'MsgBox "een=" & een
          'MsgBox "UBound(strGateway, 2)=" & UBound(strGateway, 2)
        Else
           'MsgBox "Default IP Gateway : none"
        End If

    End With
Next

'MsgBox "isch_get=" & isch_get

'если нет ни одного гетевея,то нулевой результат
If isch_get = -1 Then
 ReDim strGateway(0, 0)
End If

search_gateway = strGateway

Set collSWbemObjectSet_Win32_NetworkAdapterConfiguration = Nothing
Set objSWbemServicesEx = Nothing

End Function


