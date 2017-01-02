// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK-LABEL: entry:

int main()
{
        int len = 100;
        int *a = new int[len];
        for (int i = len; i > 0; i--)
        {
                a[i] = i;
        }
}

