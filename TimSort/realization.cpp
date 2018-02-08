// TimSort.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#include "stdafx.h"
#include <vector>
#include <iostream>
#include <clocale>
#include <conio.h>
#include <algorithm>
#include <stdlib.h>

using namespace std;

struct stackPar { //���� ���
	int start; //������ ������� ��������
	int length; //����� ����������
};

int const gallopSize = 7; //����������� ���������� �������� � ������ "������"

void InitMas(vector <int> &, int); //������������� �������

void PrintMas(vector <int> &, int); //����� �������

void Timsort(vector <int> &, int); //���������� Timsort

void Merge(vector <int> & a, stackPar &first, stackPar &second); //������� �����������

void InsertionSort(vector <int> &, int, int); //���������� ���������

int GetMinrun(int); //���������� ������������ ������� �������� ���������������� ����������

int BinarySearch(const vector<int>& container, int element);//�������� �����

int main()
{
	setlocale(0, "");
	int size = 0; //������ ��������� �������

	cout << "������� ������ ��������� �������:" << endl;
	cin >> size;
	vector <int> a(size); //��������� ������ ��������� �������

	InitMas(a, size);
	cout << "����������������� ������:" << endl;
	//PrintMas(a, size);

	int minRun = GetMinrun(size);
	cout << "����������� ������ ����������:" << endl;
	cout << minRun << endl;

	Timsort(a, size);
	cout << "��������������� ������:" << endl;
	PrintMas(a, size);

	_getch();
	return 0;
}

void InitMas(vector <int> & a, int n) {
	for (int i = 0; i < n; i++) {
		a[i] = rand() % 200 - 100; //��������� ����� � ���������� (-100;100)
	}
}

void PrintMas(vector <int> & a, int n) {
	for (int i = 0; i < n; i++) {
		cout << a[i] << " "; //������� ������� �������
	}
	cout << endl;
}

void Timsort(vector <int> & a, int n) { //�� ����� �������� ������ � ��� ������
	//���������� MINRUN
	int minRun = GetMinrun(n); //��������� minrun
   //��������� �� ���������� � �� ����������
	int sourceIndex = 0; //������� ������ ��������� ������� = 0
	vector <stackPar> subArrays; //������� ���������
	while (sourceIndex < n) { //���� �� ����� ��������� �������
		int currentIndex = sourceIndex; //������� ������ ���������� 
		bool sortType = false; // false - �� �����������, true - �� ��������
		bool run = true;//���� �� �� ��� ���������
		int elementsCount = 2; //������� ���������
		if (a[currentIndex] < a[currentIndex + 1]) sortType = false; //�������� ���������������
		else sortType = true;
		while (run) {//���� ����� ������ ���������
			currentIndex++; //����������� ������� ������
			if (sortType ? a[currentIndex] > a[currentIndex + 1] : a[currentIndex] < a[currentIndex + 1]) { //���� �����������
				elementsCount++; //����������� ���������� ���������
				continue; //��������� ����� � ����� while, ����� ������ ������ ���������
			}
			run = false; //������ �� ���� ���������, ��� ������ ��������������� �������
			if (elementsCount < minRun) { //���� ���������� ��������� � �������� ���������� ������ minrun
				if (sourceIndex + minRun >= n) //���� ������ ��������� ������� + minrun ��������� ������ ��������� �������
					elementsCount = n - sourceIndex; //��������� � ���������� = ������ ��������� - ������ ���������
				else elementsCount = minRun; //����� ��������� � ���������� = minrun
				currentIndex = sourceIndex + elementsCount - 1;//������� ������ ����� ����� ��������� ������� � ���-�� ��������� � ����������
			}
			else
				if (sortType) { //���� �� ��������
					for (int i = 0; i < elementsCount / 2; i++)
						swap(a[i], a[currentIndex - i]); //������ �������
				}
			stackPar temp; //������� �������
			temp.start = sourceIndex; //��������� ������ ������� = ������ ��������� �������
			temp.length = elementsCount; //����� = ���������� ��������� � ����������
			subArrays.push_back(temp); //���������� ������� � ��� ���������
			sourceIndex = currentIndex + 1; //���������� ������ ��������� �������
			InsertionSort(a, temp.start, currentIndex); //��������� ����������� ���������
		}
	}
	if (subArrays.size() == 1) return; //

	/*����������� ������������� ���������� ��������� ������� �������� ������� � ���������� �� ��������:
	1 - X > Y + Z; 2 - Y > Z;*/
	const int posCounter = 0; //�
	while (subArrays.size() > 1) { //���� �� ����� ����������
		if (subArrays.size() > 3) { 
			//���� �� ����������� ��� ��� ���� �� ���������� X > Y + Z � Y > Z
			while (!(subArrays[posCounter].length > subArrays[posCounter + 1].length + subArrays[posCounter + 2].length && subArrays[posCounter + 1].length > subArrays[posCounter + 2].length)) {
				if (subArrays[posCounter].length < subArrays[posCounter + 2].length) { //���� X < Z
					Merge(a, subArrays[posCounter + 1], subArrays[posCounter]); //������� Y c X
					subArrays[posCounter + 1].length += subArrays[posCounter].length; //����������� ����� ���������� Y + X
					subArrays[posCounter + 1].start = min(subArrays[posCounter + 1].start, subArrays[posCounter].start); //������� ������ ������ ����������
					subArrays.erase(subArrays.begin() + posCounter); //������� ��������� �� �
				}
				else { //���� � > Z
					Merge(a, subArrays[posCounter + 1], subArrays[posCounter + 2]); //������ Y c Z
					subArrays[posCounter + 1].length += subArrays[posCounter + 2].length; 
					subArrays[posCounter + 1].start = min(subArrays[posCounter + 1].start, subArrays[posCounter + 2].start);
					subArrays.erase(subArrays.begin() + posCounter + 2);
				}
				if (subArrays.size() < 3) break; //������� �� �����
			}
		}
		else if (subArrays.size() == 2) {
			Merge(a, subArrays[posCounter + 1], subArrays[posCounter]); //������� Y c X
			subArrays[posCounter + 1].length += subArrays[posCounter].length;
			subArrays[posCounter + 1].start = min(subArrays[posCounter + 1].start, subArrays[posCounter].start);
			subArrays.erase(subArrays.begin() + posCounter);
		}
	}
}

void Merge(vector <int> & a, stackPar &first, stackPar &second) {
	stackPar & minim = (first.length < second.length ? first : second); //������� ������� �� �����������
	stackPar & maxim = (minim.start == first.start ? second : first); //������� ������� �� �����������

	//������� ��������� ������ � �������� �������� �� �����������
	vector <int> temp(a.begin() + minim.start, a.begin() + minim.start + minim.length);
	//������� ��������� ������ � �������� �������� �� �����������, ����� ���� ������� �������� � �����������
	vector <int> max(a.begin() + maxim.start, a.begin() + maxim.start + maxim.length);
	vector <int> output; //������� ������, ������� ����� ������� ��������������
	int counterTemp = 0, counterMax = 0; //�������� �������� � ����� ������

	while (temp.size() && max.size()) { //���� �� ����� �����������
		//������� � ����� ������
		if (counterTemp == gallopSize) { //���� ���������� �������� = 7 
			int foundIt = BinarySearch(temp, max[0]) - 1; //������� �������
			if (foundIt < 0) counterTemp = 0; //���� ������ �� �������, �� �������� �������
			else {
				output.insert(output.end(), temp.begin(), temp.begin() + foundIt); //��������� � �������������� ������
				temp.erase(temp.begin(), temp.begin() + foundIt); //������� �� ���������� ���������� �������� �� ����������
			}
		}
		//������� � ����� ������
		else if (counterMax == gallopSize) { //���� ���������� �������� = 7
			int foundIt = BinarySearch(max, temp[0]) - 1; //������� �������
			if (foundIt < 0) counterMax = 0; //���� ������ �� �������, �� �������� �������
			else {
				output.insert(output.end(), max.begin(), max.begin() + foundIt); //��������� � ������������� ������
				max.erase(max.begin(), max.begin() + foundIt); //������� �� ���������� ���������� �������� �� ����������
			}
		}
		if (temp[0] < max[0]) { //���� ������ ������ ���������� < ������� ������� �������� ����������
			output.push_back(temp[0]); //��������� � �������������� ���������
			temp.erase(temp.begin());//������� �� ���������� ������� ������ �������
			counterTemp++; //����������� ������� �������� � ����� ������
		}
		else { //���� ������ ������ �������� ���������� < ������� ������� ����������
			output.push_back(max[0]); //��������� � �������������� �������
			max.erase(max.begin()); //������� �� �������� ���������� ������ �������
			counterMax++; //����������� ������� �������� � ����� ������
		}
	}

	for (int i = 0; i < temp.size(); i++) //���� �� ���������� �������
		output.push_back(temp[i]); //���������� ��� � ��������������

	for (int i = 0; i < max.size(); i++) //���� �� �������� ����������
		output.push_back(max[i]); //���������� ��� � ��������������

	for (int i = min(minim.start, maxim.start); output.size() != 0; i++) { //���� �� ��������������� �������
		a[i] = output[0]; //���������� ��� � ��������
		output.erase(output.begin()); //�������
	}
}

void InsertionSort(vector <int> & a, int start, int end) { //�� ����� �������� ������, ������ � ����� �������� ��� ����������
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
	size_t first = 0; //������ ������� ��������
	size_t last = container.size(); //������ ��������, ���������� �� ���������

	if (container.size() == 0) { //�������� �� ������� �������
		return 0;
	}
	else if (container[0] > element) { //������� ������ ���� � �������
		return 0;
	}
	else if (container[last - 1] < element) { //������� ������ ���� � �������
		return last;
	}
	while (first < last) { //���� ������ ������� ������ �������
		size_t mid = first + (last - first) / 2; //������ ��������

		if (element <= container[mid]) //������� ��������� ����� ������� ���������
			last = mid; //��������� ���������� � ��������
		else
			first = mid + 1; //������ - ��������� �� ������� ��������
	}

	if (container[last] == element) { //���� ����� �������
		return last + 1; 
	}
	else {
		return last; //����� ������ �������� - last
	}
}