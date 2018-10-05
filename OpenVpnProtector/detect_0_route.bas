Attribute VB_Name = "detect_route"
Public Function route_array(route As String) As String()
'Option Explicit
'функция возвращает массив заданных маршрутов route с численной метрикой

Dim StrokaArray() As String
Dim StrokaArray_Result() As String
Dim StrokaArray_Result_sort() As String
Dim strLine As String
Dim LastNonEmpty As Integer
Dim p As Integer
Dim k As Integer

LastNonEmpty = -1
p = -1
k = 0

'создаем массив строк с маршрутом route в начале при выводе route print
Set objShell = CreateObject("WScript.Shell")
Set objExecObject = objShell.Exec("%comspec% /c route print " & route)

Do Until objExecObject.StdOut.AtEndOfStream
 strLine = objExecObject.StdOut.ReadLine()
 'уберем пробелы в начале и в конце строки
 strLine = Trim(strLine)
  
 strIP = InStr(strLine, route)
 If strIP = 1 Then
  'разбиваем массив строки на "слова"
  StrokaArray = Split(strLine)
  'убираем пустые элементы в массиве
  For i = 0 To UBound(StrokaArray)
     If StrokaArray(i) <> "" Then
         'убираем TAB в строках
         StrokaArray(i) = Replace(StrokaArray(i), vbTab, "")
         LastNonEmpty = LastNonEmpty + 1
         StrokaArray(LastNonEmpty) = StrokaArray(i)
         'MsgBox "StrokaArray(" & LastNonEmpty & ")  " & StrokaArray(LastNonEmpty)
     End If
  Next
  'меняем размер массива с учетом убранных пустых элементов
  ReDim Preserve StrokaArray(LastNonEmpty)
  
  LastNonEmpty = -1
  
  'заносим в результирующий массив только те строки,где метрика имеет численное значение и количество элементов в строке 5
   If Val(StrokaArray(UBound(StrokaArray))) <> 0 And UBound(StrokaArray) = 4 Then
    p = p + 1
    ReDim Preserve StrokaArray_Result(4, p)
    For k = 0 To 4
     StrokaArray_Result(k, p) = StrokaArray(k)
     'MsgBox "StrokaArray(" & k & ")  ." & StrokaArray(k) & "."
    Next
   End If
   
    
 End If
Loop

'очищаем ненужный массив
ReDim StrokaArray(0)
k = k - 1

'MsgBox "k=  " & k & "p= " & p
'если строки есть хотя бы одна строка с 0.0.0.0 выводим ее, иначе обнуляем массив функции
If p > -1 Then
 'p2 = -1
 'сортируем массив от большей метрики к меньшей
 ReDim StrokaArray_Result_sort(k, p)
 For sort_index = 0 To p
  p_max = max_in_array_index(StrokaArray_Result, 4, -1)
  'p2 = p2 + 1
  If p_max = -1 Then
   MsgBox "Error:max_in_array_index = -1", vbCritical + vbSystemModal, "Safe-Inet.com"
   End
  End If
  For k2 = 0 To 4
   StrokaArray_Result_sort(k2, sort_index) = StrokaArray_Result(k2, p_max)
  Next
  'ставим вместо значения метрики -1 - не существующее значение
  StrokaArray_Result(4, p_max) = "-1"
  'MsgBox "StrokaArray_Result(4, p_max):  " & "StrokaArray_Result(4, " & p_max & ")=" & StrokaArray_Result(4, p_max)
 Next
 'очищаем ненужный массив
 ReDim StrokaArray_Result(0, 0)
 ReDim route_array(k, p)
 route_array = StrokaArray_Result_sort
Else
 ReDim route_array(0, 0)
End If

 Set objShell = Nothing
 Set objExecObject = Nothing

End Function

Private Function max_in_array_index(array_for_search, stolbik As Integer, stroka As Integer) As Integer
 'функция ищет максимум в array_for_search(stolbik,stroka) в столбце stolbik или в stroka
 'если stolbik =-1,тогда ищет в stroka и наоборот.
 'два отрицательных stolbik и stroka возвращают -1 - ошибку,иначе возвращает индекс максимума
 
 If (stolbik < 0 And stroka < 0) Or (stolbik > 0 And stroka > 0) Then
  'ошибка задания параметров
  max_in_array_index = -1
  Exit Function
 End If
 
 If (stolbik > UBound(array_for_search, 1)) Or (stroka > UBound(array_for_search, 2)) Then
  'ошибка задания параметров
  max_in_array_index = -1
  Exit Function
 End If
 
 max_in_array = 0
 max_in_array_index = -1
 
 If stolbik < 0 And (UBound(array_for_search, 2) + 1 > stroka) Then
   For stolbik_index = 0 To UBound(array_for_search, 1)
    If max_in_array < Val(array_for_search(stolbik_index, stroka)) Then
     max_in_array = Val(array_for_search(stolbik_index, stroka))
     'MsgBox "max_in_array  " & max_in_array
     max_in_array_index = stolbik_index
     'MsgBox "max_in_array_index  " & max_in_array_index
    End If
   Next
 End If
 
 If stroka < 0 And (UBound(array_for_search, 1) + 1 > stolbik) Then
   For stroka_index = 0 To UBound(array_for_search, 2)
    If max_in_array < Val(array_for_search(stolbik, stroka_index)) Then
     max_in_array = Val(array_for_search(stolbik, stroka_index))
     'MsgBox "max_in_array  " & max_in_array
     max_in_array_index = stroka_index
     'MsgBox "max_in_array_index  " & max_in_array_index
    End If
   Next
 End If
 'MsgBox "max_in_array_index:  " & max_in_array_index
End Function
