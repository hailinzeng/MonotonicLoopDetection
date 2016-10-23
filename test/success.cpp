#include <cstdio>

int __attribute__ ((noinline)) foo(int x)
{
	return x+1;
}

int main(int argc, char* argv[])
{

	//it doesn't trigger the 'exit(-1)' function because will occur OOB access
	// arr[ i ] to i >= 0 and i < sizeof(arr)
	auto t1 = []{
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 1;i < 3; i++)
		{
			arr[i] = i+3;
		}
	};
	t1();

	//it doesn't trigger the 'exit(-1)' function because will occur OOB access
	// arr[ g ] to g >= 0 and g < sizeof(arr)
	auto t2 = []{
		int g = 0;
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 1;i < 3; i++)
		{
			arr[g] = g;
			g+=1;
		}
	};
	t2();


	//it doesn't trigger the 'exit(-1)' function because will occur OOB access
	// arr[ i+z ] to i+z >= 0 and i+z < sizeof(arr)
	auto t3 = [](int arg){
		int z = arg;
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 3;i < 5; i++)
		{
			arr[i+z] = 3;
		}
	};
	t3(1);

	//it doesn't trigger the 'exit(-1)' function because will occur OOB access
	// arr[ i ] to i >= 0 and i < sizeof(arr)
	auto t4 = [](int arg){
		int arr[] = {1,2,3,4,5,6,7};
		for (int i = 1; i < arg; i++)
		{
			arr[i] = i;
		}
	};
	t4(1);

/*
	NOT MONOTONIC
	auto t5 = [](int arg){
		int arr[] = {1,2,3,4,5,6,7};
		for (; arg < 10;)
		{
			arr[arg] = 3;
			break;
		}
	};
	t5(argc);
*/

	auto t6 = [](){
		int arr[] = {1,2,3,4,5,6,7};
		int i = 0;
		while (i < 6)
		{
			arr[i] = 3;
			i++;
		}
	};
	t6();

	return 0;
}
