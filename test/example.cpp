
int main()
{
	int a[] = {1,2,3,4,5,6,7};
	for (int i = 1;i < 3; i++)
	{
		a[125-i] = i;
	}

	return 0;
}
