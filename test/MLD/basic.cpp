void function()
{
        int a[] = {1,2,3,4,5,6,7};
        int b[] = {1,2,3,4,5,6,7};
        for (int i = 1;i < 3; i++)
        {
                a[i] = b[i+1] + 1;
        }
}

