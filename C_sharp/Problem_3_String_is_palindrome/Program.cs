// Задача: Задайте произвольную строку. Выясните, является ли она палиндромом.

Console.WriteLine("Введите строку");
string str = Console.ReadLine();

bool check = true;

for (int i = 0; i < str.Length/2; i++)
{
    if (str[i] != str [str.Length - i - 1])
    {
        check = false;
        break;
    }
}

if (check)
{
    Console.WriteLine("Да");
}
else
{
    Console.WriteLine("Нет");
}