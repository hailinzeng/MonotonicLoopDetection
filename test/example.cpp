
int main(int argc, char* argv[])
{
	int a[100];
	for (int i = 1;i < 100; i--)
	{
		if(i < 25) a[i] = i+1;
		else a[125-i] = 3;
	}

	return 0;
}
