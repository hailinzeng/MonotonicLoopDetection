#include <cstdio>

int main(int argc, char** argv)
{
	int a[] = {1,2,3,4,5,6,7};
	int b[] = {1,2,3,4,5,6,7};

	int c = 100;

	for (int i = 0;i  < 5; ++i)
	{
		a[i] = b[i + 1] + c;
	}

//	for (int i=0; i<sizeof(a); i++)
//	{
//		std::printf("A[%d] = %d\n",i,a[i]);
//	}

	return a[argc];
}
