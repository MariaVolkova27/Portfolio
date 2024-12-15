// Задача 3: Задайте произвольный массив. Выведете
// его элементы, начиная с конца. Использовать
// рекурсию, не использовать циклы.

int [] array = new int [5];

void FillArray ()
{
   for (int i = 0; i < array.Length; i++)
    {
        array[i] = new Random().Next(0,25);
    }
}

void PrintArray ()
{
   for (int i = 0; i < array.GetLength(0); i++)
    {
        Console.Write(array[i]+ "\t");
    }
}

void PrintElementsOfArrayBackwards (int i)
{
    if (i < 0)
    {
        return;
    }
    Console.Write(array[i] + "\t");
    PrintElementsOfArrayBackwards(i - 1);
}

FillArray ();
PrintArray ();
Console.WriteLine();
PrintElementsOfArrayBackwards(array.Length - 1);