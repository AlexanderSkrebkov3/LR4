// ConsoleApplication15.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <sstream>
using namespace boost::filesystem;
using namespace std;
class Count // Информкация о счете
{
public:
	string broc_id; // Идентификатор брокера
	string numcount; // Номер счета
	int NumFile; // Счетчик файлов с информацией о счете
	int day, mes, year; // Дата
	bool operator>(Count & c2)  // Сравнение дат
	{
		if (year > c2.year) return true;
		else
		{
			if (year < c2.year) return false;
			if (mes > c2.mes) return true;
			else
			{
				if (mes < c2.mes) return false;
				if (day > c2.day) return true;
				else
				{
					return false;
				}

			}
		}
	}
	// Конструктор
public :	Count(string broc_id, string numcount, int day, int mes, int year)
	{
		NumFile = 1;
		this->broc_id = broc_id;
		this->numcount = numcount;
		this->day = day;
		this->mes = mes;
		this->year = year;

	}
};

int main(int argc, char *argv[])
{
	vector <Count> VC; // Вектор разных счетов
	path mainpath; // Главный каталог
	if (argc < 2) //  Параметров командной строки нет (только имя запускаемого файла)
	{
		mainpath = current_path(); // Текущий каталог
	}
	else // Параметры командной строки есть
	{
		mainpath = path(argv[1]); // Имя каталога через параметр командной строки
	}
	setlocale(LC_ALL, "rus");
	bool flag2 = false;
	cout << mainpath << endl; // Печать имени каталога
	if (!is_directory(mainpath)) // Если это не каталог, то сообщение об ошибке
	{
		cout << "Error!! Directory not found!" << endl;
		system("pause");
		return 0;
	}
	for (const directory_entry& x : directory_iterator{ mainpath }) // Цикл просмотра содержимого главного каталога
	{ // x - элемент каталога (файл или каталого)
		if (is_directory(x.path())) //  Это каталог (х) - имя каталога - это имя брокера
		{
			for (const directory_entry& y : directory_iterator{ x.path() }) // Просматриваем подкаталог (имя подкаталого - имя брокера)
			{  // y - элемент каталога (файл или каталогог)
				if (is_regular_file(y.path())) // Это файл (не каталог)
				{
					string name = y.path().stem().string(); // Имя файла (без расширения)
					string ext = y.path().extension().string(); // Расширение файла
					int pos = name.find("balance_");  // Ищет строку "balance_"
					if (pos == 0 && ext == ".txt") // Строка "balance_" начинается с начала и расширение .txt
					{
						if (name.length() == 25) // Длина имени файла всегда такая
							if (name[16] == '_') //  На этом месте всегда символ '_'
							{
								//cout << "ok" << endl;
								string numcount = name.substr(8, 8); // Извлекаем в строку номер счета
								string buf;
								//cout << numcount << endl;
								istringstream sin1(numcount); // Поток для чтения из строки
								int Num, dd, mm, yy; // Номер счета, год, месяц, день
								sin1 >> Num; // Читаем номер счета
								if (sin1.fail()) continue; // При ошибке чтения переходим к следующему файлу
								buf = name.substr(17, 4);  // Читаем в виде строки год
								//cout << buf << endl;
								istringstream sin2(buf); // Поток для чтения из строки
								sin2 >> yy; // Читаем год из строки
								if (sin2.fail()) continue; // При ошибке чтения переходим к следующему файл

								buf = name.substr(21, 2); // Читаем в виде строки номер месяца
								//cout << buf << endl;
								istringstream sin3(buf); // Поток для чтения из строки
								sin3 >> mm; // Читаем месяц из строки
								if (sin3.fail()) continue; // При ошибке чтения переходим к следующему файл

								buf = name.substr(23, 2); // Читаем в виде строки день
								//cout << buf << endl;
								istringstream sin4(buf); // Поток для чтения из строки
								sin4 >> dd; // Читаем день из строки
								if (sin4.fail()) continue; // При ошибке чтения переходим к следующему файл
								// Печать о прочитанном
								flag2 = true; // Нашли хотя бы 1 элемент, удовлетворящий требованиям
								cout << x.path().filename().string() << ' ' << y.path().filename().string() << endl;
								bool flag = false; // Флаг показываем есть такой счет или нет
								for (auto & pos : VC) // Просматриваем сохраненные счета
								{
									if (pos.numcount == numcount) // Нашли такой счет
									{
										pos.NumFile++; // Счетчик файлов
										Count C2(x.path().filename().string(), numcount, dd, mm, yy);
										if (C2 > pos) // Нашли более поздний счет
										{
											// Обновляем данные
											pos.broc_id = C2.broc_id;
											pos.day = dd;
											pos.mes = mm;
											pos.year = yy;
										}
										flag = true;
										break;

									}
								}
								if (!flag) // Такое счета не было создаем новый
								{
									Count C2(x.path().filename().string(), numcount, dd, mm, yy);
									// cout <<"1) "<< x.path().filename().string() << ' ' << numcount << ' ' << dd << ' ' << mm << ' ' << yy<<endl;
									VC.push_back(C2);
								}

							}
					}
				}
			}
		}
		//std::cout << x.path() << std::endl;
	}
	// Печать информации о найденных счетах
	for (auto pos : VC) // Просматриваем сохраненные счета
	{
		//broker:ib account:00001234 files:10 lastdate:20181017
		cout << "broker: " << pos.broc_id << " account: " << pos.numcount << " files: " << pos.NumFile << " lastdate:" << pos.year << pos.mes << pos.day << endl;
	}
	if (!flag2) // Нет элементов, удовлетворяющих требованиям
	{
		cout << "Files not found!!!" << endl;
	}
	system("pause");
	return 1;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
