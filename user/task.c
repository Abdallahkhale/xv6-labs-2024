#include <stdio.h>

int divide(int a, int b) {
    return a / b;  // Potential division by zero error
}

int main() {
    int x = 10, y = 0; // y is 0, which will cause an error
    printf("Result: %d\n", divide(x, y));
    return 0;
}

