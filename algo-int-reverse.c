#include<stdio.h>

int main() {
   int num = -1000000, new_num = 0;
   int neg = num & (1 << (sizeof(int) * 8 - 1));
   
   while (num) {
       new_num  = new_num * 10 + (num % 10);
       num     /= 10;
   }
   
   printf("%d\n", new_num | neg);
}
