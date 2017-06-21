/**
 * Problem: 
 * Find all possibilities which satisfy the equation 'abc+def=ghi', 
 * each digit is selected from 1-9, and they are all unique.
 * @author    Δοδύο
 * @version   0.1
 * @changelog 
 *      - V0.1: Basic implementation
 */

#include <stdio.h>
#include <string.h>

void comb (char remain[9], int index, int carry, char a[3], char b[3], char c[3], int *count) {
    if (index > 2) {
        int aa = a[2] * 100 + a[1] * 10 + a[0];
        int bb = b[2] * 100 + b[1] * 10 + b[0];
        int cc = c[2] * 100 + c[1] * 10 + c[0];
        (*count)++;
        printf("%d + %d = %d\n", aa, bb, cc);
        return;
    }
    int i, j;
    for (i = 0; i < 9; i++) {
        if (remain[i] == -1) continue;
        for (j = 0; j < 9; j++) {
            // exclude symmetries (symmetries cause 0 sum)
            if (remain[j] == -1 || i == j || j == 8 - i) continue;
            int cr = 0;
            char cp[9], na[3], nb[3], nc[3];
            strcpy(cp, remain);
            strcpy(na, a);
            strcpy(nb, b);
            strcpy(nc, c);

            na[index]  = remain[i];
            nb[index]  = remain[j];
            nc[index]  = remain[i] + remain[j] + carry;
            cr         = nc[index] >= 10;
            nc[index] %= 10;

            if (na[index] == nc[index] ||
                nb[index] == nc[index] || 
                nc[index] ==  0 ||        // exclude 0
                index == 2 && cr == 1)    // four digits number is impossible
                continue;
                
            int found = 0, k;
            for (k = 0; k < 9; k++) {
                if (nc[index] == cp[k]) {
                    found = 1;
                    break;
                }
            }

            if (!found) // the number does not exist in the remaining list
                continue;

            cp[i] = cp[j] = cp[k] = -1;
            comb(cp, index + 1, cr, na, nb, nc, count);
        }
    }
}

int main () {
    char set[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    char a[3]   = {-1, -1, -1};
    char b[3]   = {-1, -1, -1};
    char c[3]   = {-1, -1, -1};
    int  count = 0;
    comb(set, 0, 0, a, b, c, &count);
    printf("There are %d unique combinations.\n", count / 2);
    return 0;
}
