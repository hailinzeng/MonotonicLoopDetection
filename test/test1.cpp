#include <cstdio>

int __attribute__ ((noinline)) foo(int x)
{
	return x+1;
}

//monotonic loop
void test_sucess_1()
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i = 1;i < 3; i++)
	{
		arr[i] = i+3;
	}
}

//monotonic loop
void test_sucess_2()
{
	int g = 3;
	int arr[] = {1,2,3,4,5,6,7};
	for (int i = 1;i < 3; i++)
	{
		arr[g] = g;
		g+=2;
	}
}

//monotonic loop but array's index uses function parameter
void test_fail_1(int arg)
{
	int z = arg;
	int arr[] = {1,2,3,4,5,6,7};
	for (int i = 3;i < 5; i++)
	{
		arr[i+z] = 3;
	}
}

//not monotonic loop, uses function parameter
void test_fail_2(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i = 1; i < argc; i++)
	{
		arr[i] = i;
	}
}

//monotonic loop, but array uses function result
void test_fail_3(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i = 1; i < 3; i++)
	{
		arr[foo(argc+i)] = i + 1;
	}
}

//cannot get the end value
void test_fail_4()
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i = 1; ; i++)
	{
		arr[i] = i + 1;
	}
}

//cannot get the end value
void test_fail_5()
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i=0; ;)
	{
		arr[i] = i+1;
	}
}

//condition uses function parameter
void test_fail_6(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i=0; argc<5; i++)
	{
		arr[i] = i + 1;
	}
}

//condition uses function parameter
void test_fail_7(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i=0; 5<argc; i++)
	{
		arr[i] = i + 1;
	}
}

//start value is function parameter
void test_fail_8(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i=argc; i<10; i++)
	{
		arr[i] = i + 1;
	}
}

//end value is not constant
void test_fail_9(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};

	int j = 3;
	if(argc < 2)
	{
		j = 4;
	}

	for (int i=0; i<j; i++)
	{
		arr[i] = i + 1;
	}
}

//start value is not constant
void test_fail_11(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};

	int j = 3;
	if(argc < 2)
	{
		j = 4;
	}

	for (int i=j; i<10; i++)
	{
		arr[i] = i + 1;
	}
}

//cannot get start and end values
void test_fail_12()
{
	int arr[] = {1,2,3,4,5,6,7};
	int i = 10;
	for ( ; ; )
	{
		arr[i] = 12;
		i++;
	}

}

void test_fail_13()
{
	int arr[] = {1,2,3,4,5,6,7};
	int k = 10;
	for (int l = 0; l<k; l++)
	{
		arr[l * 2 + k] = 12;
	}
}

void test_fail_14(int arg)
{
	int arr[] = {1,2,3,4,5,6,7};

	int k = 10;

	if(arg==2) k = 3;

	for (int l = 0; l<k; l++)
	{
		arr[l * 2 + k] = 12;
	}
}

void test_fail_15(int arg)
{
	int arr[] = {1,2,3,4,5,6,7};
	int k = foo(1);
	for (int l = 0; l<k; l++)
	{
		arr[l * 2 + k] = 12;
	}
}






