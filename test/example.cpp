int main(int argc, char** argv)
{
	int arr[100];

	for (int i = 0 ; i < 51; i++) {
        	arr[2*i + 2 + 4 * 5 - i * 9 * 12 - 2] = i;
	}
	return 0;
}
