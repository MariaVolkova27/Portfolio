// Задача: Напишите программу, которая на вход
// принимает натуральное число N, а на выходе
// показывает его цифры через запятую.
// 568 => 5,6,8
// 8 => 8
// 9542 => 9,5,4,2

Console.WriteLine("Введите число");
int num = Convert.ToInt32 (Console.ReadLine());

int digits = 1;
int a = num;

while (a >= 10)
{
    digits++;
    a = a / 10;
}

double result = 0;
double b = num;

for (int i = digits; i >  0; i--)
{
    result = (b - b % Math.Pow(10,i-1)) / Math.Pow(10,i-1);

    Console.Write($"{result} ");

    b = b - result * Math.Pow(10,i-1);
}
