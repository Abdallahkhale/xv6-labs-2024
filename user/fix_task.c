#include <stdio.h>

int divide(int a, int b) {
    if (b == 0) {
        printf("Error: Division by zero is not allowed.\n");
        return 0;  // Return a default value instead of crashing
    }
    return a / b;
}

int main() {
    int x = 10, y = 0; // y is 0, which was causing an error
    printf("Result: %d\n", divide(x, y));
    return 0;
}

