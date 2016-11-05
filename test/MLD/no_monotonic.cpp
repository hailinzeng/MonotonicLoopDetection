void function()
{
	int arr[100];
	for(unsigned int i = 0; i < 10; i++)
	{
		arr[i] = 1 - i;
	}

	for (int i = 0 ; i < 50; i++) {
	       arr[125 - i] = i;
	}
}
