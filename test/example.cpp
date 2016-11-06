void function()
{
    int arr[100]; 
    for (int i = 0 ; i < 50; i++) {    
       if (i < 25)    
          arr[i] = i;
           else 
          arr[125 - i] = i;
    }

    for (int i = 0 ; i < 15; i++) {        
       arr[(i%12)] = i;
    }
}



