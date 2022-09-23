#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void case_0(int *histogram) {
    histogram[0]++;
}
void case_1(int *histogram) {
    histogram[1]++;
}
void case_2(int *histogram) {
    histogram[2]++;
}
void case_3(int *histogram) {
    histogram[3]++;
}
void case_4(int *histogram) {
    histogram[4]++;
}
void case_5(int *histogram) {
    histogram[5]++;
}
void case_6(int *histogram) {
    histogram[6]++;
}
void case_7(int *histogram) {
    histogram[7]++;
}
void case_8(int *histogram) {
    histogram[8]++;
}
void case_9(int *histogram) {
    histogram[8]++;
}
void case_10(int *histogram) {
    histogram[10]++;
}
void case_11(int *histogram) {
    histogram[11]++;
}
void case_12(int *histogram) {
    histogram[12]++;
}
void case_13(int *histogram) {
    histogram[13]++;
}

int main() {
    int *array = NULL;
    int count = 0;
    for (int k = 0; k < 1000000; k++) {
        size_t length = 1 << 10;
        array = realloc(array, length * 4);
        for (size_t i = 0; i < length; i++) {
            array[i] = 12345 + k;
        }
        for (int j = 0; j < 20; j++) {
            // fprintf(stderr, "allocating array: %ld\n", length * 4);
            array = realloc(array, length * 4);
            count++;
            char *x = malloc(j * 2);
            char *y = malloc(j * 4);
            char *z = malloc(j * 8);
            free(x);
            free(y);
            free(z);
            length *= 2;
        }
    }

    printf("final count %i\n", count);

    free(array);
    return 0;
}
