// TimSort.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <vector>
#include <iostream>
#include <clocale>
#include <conio.h>
#include <algorithm>
#include <stdlib.h>

using namespace std;

struct stackPar { //стэк пар
	int start; //индекс первого элемента
	int length; //длина подмассива
};

int const gallopSize = 7; //минимальное количество итераций в режиме "галопа"

void InitMas(vector <int> &, int); //инициализация массива

void PrintMas(vector <int> &, int); //вывод массива

void Timsort(vector <int> &, int); //сортировка Timsort

void Merge(vector <int> & a, stackPar &first, stackPar &second); //слияние подмассивов

void InsertionSort(vector <int> &, int, int); //сортировка вставками

int GetMinrun(int); //вычисление минимального размера частично отсортированного подмассива

int BinarySearch(const vector<int>& container, int element);//бинарный поиск

int main()
{
	setlocale(0, "");
	int size = 0; //размер исходного массива

	cout << "Введите размер исходного массива:" << endl;
	cin >> size;
	vector <int> a(size); //выделение памяти исходному массиву

	InitMas(a, size);
	cout << "Неотсортированный массив:" << endl;
	//PrintMas(a, size);

	int minRun = GetMinrun(size);
	cout << "Минимальный размер подмассива:" << endl;
	cout << minRun << endl;

	Timsort(a, size);
	cout << "Отсортированный массив:" << endl;
	PrintMas(a, size);

	_getch();
	return 0;
}

void InitMas(vector <int> & a, int n) {
	for (int i = 0; i < n; i++) {
		a[i] = rand() % 200 - 100; //разномные числа в промежутке (-100;100)
	}
}

void PrintMas(vector <int> & a, int n) {
	for (int i = 0; i < n; i++) {
		cout << a[i] << " "; //выводим элемент массива
	}
	cout << endl;
}

void Timsort(vector <int> & a, int n) { //на входе исходный массив и его размер
	//ВЫЧИСЛЕНИЕ MINRUN
	int minRun = GetMinrun(n); //вычисляем minrun
   //РАЗБИЕНИЕ НА ПОДМАССИВЫ И ИХ СОРТИРОВКА
	int sourceIndex = 0; //текущий индекс исходного массива = 0
	vector <stackPar> subArrays; //создаем подмассив
	while (sourceIndex < n) { //пока не конец исходного массива
		int currentIndex = sourceIndex; //текущий индекс подмассива 
		bool sortType = false; // false - по возрастанию, true - по убыванию
		bool run = true;//ищем ли мы ещё подмассив
		int elementsCount = 2; //счетчик элементов
		if (a[currentIndex] < a[currentIndex + 1]) sortType = false; //проверка упорядоченности
		else sortType = true;
		while (run) {//пока нужно искать подмассив
			currentIndex++; //увеличиваем текущий индекс
			if (sortType ? a[currentIndex] > a[currentIndex + 1] : a[currentIndex] < a[currentIndex + 1]) { //если упорядочено
				elementsCount++; //увиличиваем количество элементов
				continue; //переходим снова к циклу while, чтобы дальше искать подмассив
			}
			run = false; //больше не ищем подмассив, как только упорядоченность пропала
			if (elementsCount < minRun) { //если количество элементов в найденно подмассиве меньше minrun
				if (sourceIndex + minRun >= n) //если индекс исходного массива + minrun превышает размер исходного массива
					elementsCount = n - sourceIndex; //элементов в подмассиве = размер исходного - индекс исходного
				else elementsCount = minRun; //иначе элементов в подмассиве = minrun
				currentIndex = sourceIndex + elementsCount - 1;//текущий индекс равен сумме исходного индекса и кол-ву элементов в подмассиве
			}
			else
				if (sortType) { //если по убыванию
					for (int i = 0; i < elementsCount / 2; i++)
						swap(a[i], a[currentIndex - i]); //меняем местами
				}
			stackPar temp; //создаем отрезок
			temp.start = sourceIndex; //начальный индекс отрезка = индекс исходного массива
			temp.length = elementsCount; //длина = количеству элементов в подмассиве
			subArrays.push_back(temp); //закидываем отрезок в наш подмассив
			sourceIndex = currentIndex + 1; //перемещаем индекс исходного массива
			InsertionSort(a, temp.start, currentIndex); //сортируем сортировкой вставками
		}
	}
	if (subArrays.size() == 1) return; //

	/*ОПРЕДЕЛЕНИЕ НЕОБХОДИМОСТИ ВЫПОЛНЕНИЯ ПРОЦЕДУРЫ СЛИЯНИЯ ТЕКУЩЕГО МАССИВА С ПРЕДЫДУЩИМ ПО ПРАВИЛАМ:
	1 - X > Y + Z; 2 - Y > Z;*/
	const int posCounter = 0; //Х
	while (subArrays.size() > 1) { //пока не конец подмассива
		if (subArrays.size() > 3) { 
			//пока не выполняется оба или одно из требования X > Y + Z и Y > Z
			while (!(subArrays[posCounter].length > subArrays[posCounter + 1].length + subArrays[posCounter + 2].length && subArrays[posCounter + 1].length > subArrays[posCounter + 2].length)) {
				if (subArrays[posCounter].length < subArrays[posCounter + 2].length) { //если X < Z
					Merge(a, subArrays[posCounter + 1], subArrays[posCounter]); //сливаем Y c X
					subArrays[posCounter + 1].length += subArrays[posCounter].length; //увеличиваем длину подмассива Y + X
					subArrays[posCounter + 1].start = min(subArrays[posCounter + 1].start, subArrays[posCounter].start); //находим первый индекс подмассива
					subArrays.erase(subArrays.begin() + posCounter); //очищаем подмассив до Х
				}
				else { //если Х > Z
					Merge(a, subArrays[posCounter + 1], subArrays[posCounter + 2]); //слиаем Y c Z
					subArrays[posCounter + 1].length += subArrays[posCounter + 2].length; 
					subArrays[posCounter + 1].start = min(subArrays[posCounter + 1].start, subArrays[posCounter + 2].start);
					subArrays.erase(subArrays.begin() + posCounter + 2);
				}
				if (subArrays.size() < 3) break; //выходим из цикла
			}
		}
		else if (subArrays.size() == 2) {
			Merge(a, subArrays[posCounter + 1], subArrays[posCounter]); //сливаем Y c X
			subArrays[posCounter + 1].length += subArrays[posCounter].length;
			subArrays[posCounter + 1].start = min(subArrays[posCounter + 1].start, subArrays[posCounter].start);
			subArrays.erase(subArrays.begin() + posCounter);
		}
	}
}

void Merge(vector <int> & a, stackPar &first, stackPar &second) {
	stackPar & minim = (first.length < second.length ? first : second); //находим меньший из подмассивов
	stackPar & maxim = (minim.start == first.start ? second : first); //находим больший из подмассивов

	//создаем временный массив с размером меньшего из подмассивов
	vector <int> temp(a.begin() + minim.start, a.begin() + minim.start + minim.length);
	//создаем временный массив с размером большего из подмассивов, чтобы было удобнее работать с подмассивом
	vector <int> max(a.begin() + maxim.start, a.begin() + maxim.start + maxim.length);
	vector <int> output; //создаем массив, который будет считать результирующим
	int counterTemp = 0, counterMax = 0; //счетчики перехода в режим галопа

	while (temp.size() && max.size()) { //пока не конец подмассивов
		//ПЕРЕХОД В РЕЖИМ ГАЛОПА
		if (counterTemp == gallopSize) { //если количество итераций = 7 
			int foundIt = BinarySearch(temp, max[0]) - 1; //находим элемент
			if (foundIt < 0) counterTemp = 0; //если индекс не нашелся, то обнуляем счетчик
			else {
				output.insert(output.end(), temp.begin(), temp.begin() + foundIt); //вставляем в результирующий массив
				temp.erase(temp.begin(), temp.begin() + foundIt); //очищаем из временного подмассива элементы до найденного
			}
		}
		//ПЕРЕХОД В РЕЖИМ ГАЛОПА
		else if (counterMax == gallopSize) { //если количество итераций = 7
			int foundIt = BinarySearch(max, temp[0]) - 1; //находим элемент
			if (foundIt < 0) counterMax = 0; //если индекс не нашелся, то обнуляем счетчик
			else {
				output.insert(output.end(), max.begin(), max.begin() + foundIt); //всьавляем в резльтирующий массив
				max.erase(max.begin(), max.begin() + foundIt); //очищаем из временного подмассива элементы до найденного
			}
		}
		if (temp[0] < max[0]) { //если первый индекс временного < первого индекса большего подмассива
			output.push_back(temp[0]); //вставляем в результирующий временный
			temp.erase(temp.begin());//очищаем из временного массива первый элемент
			counterTemp++; //увеличиваем счетчик перехода в режим галопа
		}
		else { //если первый индекс большего подмассива < первого индекса временного
			output.push_back(max[0]); //вставляем в результирующий больший
			max.erase(max.begin()); //очищаем из большего подмассива первый элемент
			counterMax++; //увеличиваем счетчик перехода в режим галопа
		}
	}

	for (int i = 0; i < temp.size(); i++) //идем по временному массиву
		output.push_back(temp[i]); //закидываем его в результирующий

	for (int i = 0; i < max.size(); i++) //идем по большему подмассиву
		output.push_back(max[i]); //закидываем его в результирующий

	for (int i = min(minim.start, maxim.start); output.size() != 0; i++) { //идем по результирующему массиву
		a[i] = output[0]; //закидываем его в исходный
		output.erase(output.begin()); //очищаем
	}
}

void InsertionSort(vector <int> & a, int start, int end) { //на входе исходный массив, начало и конец индексов для сортировки
	for (int i = 1 + start; i < end + 1; i++) {
		int x = a[i];
		int j = i;
		while (j > start && a[j - 1] > x) {
			a[j] = a[j - 1];
			j--;
		}
		a[j] = x;
	}
}

int GetMinrun(int n) {
	int r = 0;
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

int BinarySearch(const vector<int>& container, int element) {
	size_t first = 0; //индекс первого элемента
	size_t last = container.size(); //индекс элемента, следующего за последним

	if (container.size() == 0) { //проверка на пустоту массива
		return 0;
	}
	else if (container[0] > element) { //элемент меньше всех в массиве
		return 0;
	}
	else if (container[last - 1] < element) { //элемент больше всех в массиве
		return last;
	}
	while (first < last) { //пока первый элемент меньше размера
		size_t mid = first + (last - first) / 2; //индекс середины

		if (element <= container[mid]) //элемент находится перед средним элементом
			last = mid; //последний становится в середину
		else
			first = mid + 1; //первый - следующий за средним индексом
	}

	if (container[last] == element) { //если нашли элемент
		return last + 1; 
	}
	else {
		return last; //иначе индекс элемента - last
	}
}