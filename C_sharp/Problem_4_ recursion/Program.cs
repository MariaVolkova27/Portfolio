// Задача 1: Задайте значения M и N. Напишите
// программу, которая выведет все натуральные числа
// в промежутке от M до N. Использовать рекурсию, не
// использовать циклы.

Console.WriteLine("Введите число M");
int numberM = int.Parse(Console.ReadLine());
Console.WriteLine("Введите число N");
int numberN = int.Parse(Console.ReadLine());

int minNumber = 0; // наименьшее из двух чисел
int maxNumber = 0; // наибольшее из двух чисел

if (numberM > numberN)
{
    minNumber = numberN;
    maxNumber = numberM;
}
else
{
    minNumber = numberM;
    maxNumber = numberN;
}

void PrintNumbers (int minNum, int maxNum)
{
    if (minNum > maxNum)
    {
        return;
    }
    Console.Write(minNum + "\t");
    PrintNumbers(++minNum,maxNum);
}

PrintNumbers(minNumber,maxNumber);