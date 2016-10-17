#include <cstdio>

int __attribute__ ((noinline)) foo(int x)
{
	return x+1;
}

int main(int argc, char** argv)
{

	int z = argc;
	int a[] = {1,2,3,4,5,6,z};
	int b[] = {1,2,3,4,5,6,z};
	int c[] = {1,2,3,4,5,6,z};
	int d[] = {1,2,3,4,5,6,z};
	int e[] = {1,2,3,4,5,6,z};

	for (int i = 3;i < 5; i++)
	{
		a[i+z] = 3;
	}

	for (int j = 1;j < 3; j++)
	{
		b[j] = j+3;
	}

	int g = 3;

	for (int k = 1;k < 3; k++)
	{
		c[g] = g;
		g+=2;
	}

	for (int l = 1; ; l++)
	{
		d[l] = l + 1;
	}
/*
	for (int m = 1; m < 3; m++)
	{
		e[foo(argc+m)] = m + 1;
	}

*/


	return a[argc];
}
