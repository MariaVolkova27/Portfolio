// Задача 2: Напишите программу вычисления функции
// Аккермана с помощью рекурсии. Даны два
// неотрицательных числа m и n.

Console.WriteLine("Введите число m");
int numberM = int.Parse(Console.ReadLine());
Console.WriteLine("Введите число n");
int numberN = int.Parse(Console.ReadLine());

int AkkermanFunction (int numM, int numN)
{
    if (numM == 0)
    {
        return numN + 1;
    }
    else if (numN == 0 && numM > 0)
    {
        return AkkermanFunction(numM - 1, 1);
    }
    else
    {
        return AkkermanFunction(numM - 1, AkkermanFunction(numM, numN - 1));
    }
}

Console.WriteLine(AkkermanFunction(numberM,numberN));
