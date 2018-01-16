#include <iostream>

#include "functions.h"

using namespace std;

int main()
{
	cout << "Hello World!" << endl;
	system("pause");
	return 0;
}

extern "C"
{
    DLL_PUBLIC int add(int a, int b) {
        return a + b;
    }
}