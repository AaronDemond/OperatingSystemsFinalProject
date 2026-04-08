#include <stdio.h>
#include "logger.h"

int main()
{
    double P;
    long long N;

    LOG("Amdahl module started");

    // Input parallel fraction
    while (1) {
        printf("Enter parallel fraction (0-1): ");
        if (scanf("%lf", &P) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        if (P < 0 || P > 1) {
            printf("P must be between 0 and 1.\n");
            continue;
        }
        break;
    }
    LOGF("Parallel fraction selected: %.4f", P);

    // Input number of cores
    while (1) {
        printf("Enter number of cores: ");
        if (scanf("%lld", &N) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        if (N < 1) {
            printf("Number of cores must be >= 1.\n");
            continue;
        }
        break;
    }

    // Amdahl's Law
    double parallel_time = (1 - P) + (P / N);
    double speedup = 1 / parallel_time;

    // Output
    printf("\n--- Results ---\n");
    printf("Serial Execution Time: 1.0000\n");
    printf("Parallel Execution Time: %.4f\n", parallel_time);
    printf("Speedup: %.4fx\n", speedup);

    LOGF("Speedup Calculated: %.4f", speedup);
    LOG("Amdahl speedup calculated");


    return 0;
}
