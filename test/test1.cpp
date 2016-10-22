#include <cstdio>

int __attribute__ ((noinline)) foo(int x)
{
	return x+1;
}

int main(int argc, char* argv[])
{


	auto t1 = []{
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 1;i < 3; i++)
		{
			arr[i] = i+3;
		}
	};

	t1();

	auto t2 = []{
		int g = 3;
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 1;i < 3; i++)
		{
			arr[g] = g;
			g+=2;
		}
	};

	t2();

	auto t3 = [](int arg){
		int z = arg;
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 3;i < 5; i++)
		{
			arr[i+z] = 3;
		}
	};

	t3(argc);

	auto t4 = [](int arg){
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 1; i < arg; i++)
		{
			arr[i] = i;
		}
	};

	t4(argc);

	auto t5 = [](int arg){
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 1; i < 3; i++)
		{
			arr[foo(arg+i)] = i + 1;
		}
	};

	t5(argc);

/*

	auto t6 = [](){
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 1; ; i++)
		{
			arr[i] = i + 1;
		}
	};

	t6();

	auto t7 = [](){
		int arr[] = {1,2,3,4,5,6,7};
		for (int i=0; ;)
		{
			arr[i] = i+1;
		}
	};

	t7();
*/
/*
void test8(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i=0; argc<5; i++)
	{
		arr[i] = i + 1;
	}
}

void test_fail_7(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i=0; 5<argc; i++)
	{
		arr[i] = i + 1;
	}
}

void test_fail_8(int argc)
{
	int arr[] = {1,2,3,4,5,6,7};
	for (int i=argc; i<10; i++)
	{
		arr[i] = i + 1;
	}
}

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
*/


	return 0;
}
