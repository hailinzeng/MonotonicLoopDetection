#include <cstdio>

int main(int argc, char** argv)
{

	int z = argc + 1;

	int a[] = {1,2,3,4,5,6,7};
	int b[] = {1,2,3,4,5,6,z};

	int c = 100;

	for (int i = 3;i < 5; i++)
	{
		a[i] = b[z + 1] + c;
	}

	z += a[0];

	return z;
}
