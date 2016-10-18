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
	int f[] = {1,2,3,4,5,6,z};

	//unsafe
	for (int i = 3;i < 5; i++)
	{
		a[i+z] = 3;
	}

	//safe
	for (int j = 1;j < 3; j++)
	{
		b[j] = j+3;
	}

	int g = 3;

	//safe
	for (int k = 1;k < 3; k++)
	{
		c[g] = g;
		g+=2;
	}

	//unsafe
	for (int m = 1; m < 3; m++)
	{
		d[foo(argc+m)] = m + 1;
	}

	//unsafe
	for (int m2 = 1; m2 < 3; m2++)
	{
		e[argc] = m2 + 1;
	}

	//safe
	for (int l = 1; ; l++)
	{
		f[l] = l + 1;
	}


	return a[argc];
}
