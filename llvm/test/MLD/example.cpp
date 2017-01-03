#include <stdlib.h>
int main() {
  int size = 32 + rand()%10;
  int step = size % 8;
  char* str = new char[size];
  for (int i = 0; i + step < size; i+=step) {
    for (int j = 0; j < step; j++) {
      str[i+j] = str[i+j] + i + j;
    }
  }
  return 0;
}
