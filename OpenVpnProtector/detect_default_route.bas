Attribute VB_Name = "detect_default_route"
Public Function default_route_index(Arr_0() As String) As Integer
'Option Explicit
'������� � ���������� ������� ������� ��������� ���� ������� � ��������� �������� � ���������� �������� �������
'����� �������� � �������,���� ��� �� ������ �������� ��������,�� ���������� -1

Dim Metrica_temp As Integer
Dim Metrica As Long


Metrica = 999999
Metrica_temp = 0
flag = -1

'���� ���� ���� �� 1 ������� 0.0.0.0,�� ���� ������� � ���������� ��������
If Arr_0(0, 0) <> "" Then
 For i = 0 To UBound(Arr_0, 2)
  Metrica_temp = Val(Arr_0(UBound(Arr_0, 1), i))
  If Metrica_temp < Metrica Then
   Metrica = Metrica_temp
   'StrokaArray_Result = StrokaArray
   flag = i
  End If
 Next
End If
 default_route_index = flag
   
End Function

