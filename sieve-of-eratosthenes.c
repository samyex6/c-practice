/**
 * Implementation of the prime generation algo - Sieve of Eratosthenes
 * @author    Δοδύο
 * @version   0.1
 * @changelog
 *      - V0.1: Basic implementation
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define SIZE (UINT_MAX / 80)

char *arr;

void soe (char *a, size_t size) {
    size_t i, j, k;
    for (i = 2, j = sqrt(size); i <= j; i++) {
        if (!arr[i - 1]) continue;
        for (k = i * i; k <= size; k += i) {
            arr[k - 1] = 0;
        }
    }
}

int main () {
    size_t  i, j = 1;
    clock_t begin;

    // Sieve of Eratosthenes
    begin = clock();
    arr   = (char *) malloc(sizeof(char) * SIZE);
    memset(arr, 1, SIZE);
    soe(arr, SIZE);

    /*for (i = 2; i <= SIZE; i++) {
        if (arr[i - 1]) {
            printf("%d ", i);
            if (!(j++ % 10)) {
                printf("\n");
            }
        }
    }
    printf("\n");*/
    printf("\nTime spent: %lfsecs\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
    
    return 0;
}
