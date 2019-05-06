#include <iostream>
#include <conio.h>
#include <Windows.h>

HANDLE event;
const int BIG_NUMBER = 999999;

struct worker_array
{
	worker_array() :a(nullptr), n(0), sum(0) {}
	int *a; // - элементы массива
	int n; // - кол-во элементов
	int sum; // - требуемая сумма
};

DWORD WINAPI worker_2(void *lpparameters)
{
	worker_array* arr = (worker_array*) lpparameters;
	int min = BIG_NUMBER;
	for (int i = 0; i < arr->n; i++)
	{
		if (arr->a[i] < min)
		{
			min = arr->a[i];
		}
		if (arr->a[i] % 2 != 0)
			arr->sum += arr->a[i];
	}
	if (min % 2 == 0)
	{
		arr->sum += min;
	}
	return 0;
}
int ass(int*, const int);

DWORD WINAPI worker(void *lpParameters)
{
	WaitForSingleObject(event, 1);
	worker_array* arr = (worker_array*)lpParameters;
	arr->sum = ass(arr->a, arr->n);
	return 0;
}

int ass(int *a, const int n)
{
	int min = BIG_NUMBER;
	int sum = 0;
	__asm
	{
		mov ecx, n
		mov ebx, 0
		mov edx,0
		loop_metka:
		mov edx, a
		mov ebx, dword ptr [edx]
			cmp min, ebx
			jg min_metka
			ret_metka :
		test ebx, 1
			jne even_metka
			ret2_metka :
		add a, 4
			loop loop_metka
			jmp exit_metka
			min_metka :
		mov dword ptr min, ebx
			jmp ret_metka
			even_metka :
		add dword ptr sum, ebx
			jmp ret2_metka
			exit_metka :
		mov ebx, dword ptr min
			test ebx, 1
			je min_odd_metka
			jmp exit_exit
			min_odd_metka :
		add dword ptr sum, ebx
			exit_exit :
	}
	return sum;
}

int main()
{
	worker_array arr;
	worker_array arr2;
	std::cout << "Number of arr elements: ";
	std::cin >> arr.n;
	arr.a = new int[arr.n];
	std::cout << "\nElements of arr:";
	for (int i = 0; i < arr.n; i++)
	{
		std::cin >> arr.a[i];
	}
	DWORD thrID;
	event = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE thread = CreateThread(NULL, 0, &worker, &arr, 0, &thrID);
	if (_getch() == '+')
	{
		SetEvent(event);
	}
	WaitForSingleObject(thread, INFINITE);
	std::cout << "\nsum = " << arr.sum << "\n";
	delete[]arr.a;
	std::system("pause");
	return 0;
}