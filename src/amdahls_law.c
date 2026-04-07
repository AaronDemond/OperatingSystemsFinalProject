#include <stdio.h>
#include "logger.h"

int main()
{
    double P;
    int N;

    LOG("Amdahl module started");

    // Input
    printf("Enter parallel fraction (0-1): ");
    scanf("%lf", &P);
    LOGF("Parallel fraction selected: %.2f", P);


    if (P < 0 || P > 1)
    {
        printf("Invalid input. P must be between 0 and 1.\n");
        return 1;
    }

    printf("Enter number of cores: ");
    scanf("%d", &N);
    LOGF("Number of cores selected: %d", N);


    if (N < 1)
    {
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

    LOGF("Speedup Calculated: %.2f", speedup);
    LOG("Amdahl speedup calculated");


    return 0;
}
