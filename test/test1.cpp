#include <cstdio>

int __attribute__ ((noinline)) foo(int x)
{
	return x+1;
}

//unsafe: uses function args
void test1(int arg)
{
	int z = arg;
	int arr1[] = {1,2,3,4,5,6,7};
	for (int i = 3;i < 5; i++)
	{
		arr1[i+z] = 3;
	}
}

//safe: index variable don't depend of unsafe values
void test2()
{
	int arr2[] = {1,2,3,4,5,6,7};
	for (int i = 1;i < 3; i++)
	{
		arr2[i] = i+3;
	}
}

//safe: index variable is not the loop variable but don't depend of unsafe values
void test3()
{
	int g = 3;
	int arr3[] = {1,2,3,4,5,6,7};
	for (int i = 1;i < 3; i++)
	{
		arr3[g] = g;
		g+=2;
	}
}

//unsafe: index variable is a function result
void test4(int argc)
{
	int arr4[] = {1,2,3,4,5,6,7};
	for (int i = 1; i < 3; i++)
	{
		arr4[foo(argc+i)] = i + 1;
	}
}

//unsafe: uses function args
void test5(int argc)
{
	int arr5[] = {1,2,3,4,5,6,7};
	for (int i = 1; i < 3; i++)
	{
		arr5[argc] = i + 1;
	}
}

//unsafe: cannot get the end of the loop
void test6()
{
	int arr6[] = {1,2,3,4,5,6,7};
	for (int i = 1; ; i++)
	{
		arr6[i] = i + 1;
	}
}

//unsafe: cannot get the end of the loop
void test7()
{
	int arr7[] = {1,2,3,4,5,6,7};
	for (int i=0; ;)
	{
		arr7[i] = i+1;
	}
}

//safe: index uses function parameter but still can get the start and begin of the loop
void test8(int argc)
{
	int arr8[] = {1,2,3,4,5,6,7};
	for (int i=0; argc<5; i++)
	{
		arr8[i] = i + 1;
	}
}

//unsafe: index uses function parameter, cannot detect the end of the loop
void test9(int argc)
{
	int arr9[] = {1,2,3,4,5,6,7};
	for (int i=0; 5<argc; i++)
	{
		arr9[i] = i + 1;
	}
}

//unsafe: start variable is function argument or function
void test10(int argc)
{
	int arr10[] = {1,2,3,4,5,6,7};
	for (int i=argc; i<10; i++)
	{
		arr10[i] = i + 1;
	}
}

int main(int argc, char** argv)
{

	test1(argc);
	test2();
	test3();
	test4(argc);
	test5(argc);
	test6();
	test7();
	test8(argc);
	test9(argc);
	test10(argc);

	return argc;
}
