int main(int argc, char** argv)
{
	int foo[10];
	int j = 1;
	for(int i=0; i<10; i++)
	{
		foo[j] = argc;
		j+=2;
	}
	return 0;
}
