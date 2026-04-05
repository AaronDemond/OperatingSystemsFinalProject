#include <stdio.h>
#include "logger.h"

int main() {
    double P;
    int N;

    write_log("AMDAHL", "Module started");

    // Input
    printf("Enter parallel fraction (0-1): ");
    scanf("%lf", &P);
    write_log_input("AMDAHL", "parallel_fraction", P);

    if (P < 0 || P > 1) {
        write_log("AMDAHL", "Invalid parallel fraction");
        printf("Invalid input. P must be between 0 and 1.\n");
        return 1;
    }

    printf("Enter number of cores: ");
    scanf("%d", &N);
    write_log_input("AMDAHL", "number_of_cores", N);

    if (N < 1) {
        write_log("AMDAHL", "Invalid number of cores");
        printf("Invalid input. Number of cores must be >= 1.\n");
        return 1;
    }

    // Amdahl's Law
    double parallel_time = (1 - P) + (P / N);
    double speedup = 1 / parallel_time;

    // Output
    printf("\n--- Results ---\n");
    printf("Serial Execution Time: 1.00\n");
    printf("Parallel Execution Time: %.2f\n", parallel_time);
    printf("Speedup: %.2fx\n", speedup);

    char speedup_str[32];
    snprintf(speedup_str, sizeof(speedup_str), "%.2fx", speedup);
    write_log_input("AMDAHL", "speedup", speedup_str);
    write_log("AMDAHL", "Calculation complete");

    return 0;
}
