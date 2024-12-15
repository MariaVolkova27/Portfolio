// Задание
// Задайте двумерный массив из целых чисел. Сформируйте новый 
// одномерный массив, состоящий из средних арифметических
// значений по строкам двумерного массива.

int [,] array = new int [3,4];

void FillArray ()
{
   for (int i = 0; i < array.GetLength(0); i++)
    {
        for (int j = 0; j < array.GetLength(1); j++)
        {
        array[i,j] = new Random().Next(0,10);
        }
    }
}

void Print2DArray (int [,] matrix)
{
   for (int i = 0; i < matrix.GetLength(0); i++)
    {
        for (int j = 0; j < matrix.GetLength(1); j++)
        {
        Console.Write(matrix[i,j]+ " \t");
        }
        
        Console.WriteLine();
    }
}

void Print1DArray (double [] matrix)
{
   for (int i = 0; i < matrix.Length; i++)
    {
        Console.Write(matrix[i]+ " \t");
    }
}

double [] ArrayOfMeanElements ()
{
    double [] matrix = new double [array.GetLength(0)];
    for (int i = 0; i < array.GetLength(0); i++)
    {
        double sum = 0; 
        for (int j = 0; j < array.GetLength(1); j++)
        {
            sum += array[i,j];
        }
        matrix [i] = sum / array.GetLength(1);
        }
    return matrix;
}


FillArray();
Print2DArray(array);
Console.WriteLine();

double [] newArray = ArrayOfMeanElements ();
Print1DArray(newArray);






