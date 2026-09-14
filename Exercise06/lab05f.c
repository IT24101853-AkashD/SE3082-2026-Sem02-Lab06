#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000
#define STRIP_SIZE 256  // Strip size aligned with cache/vector characteristics

int main() {
    double *A = (double *)malloc(N * sizeof(double));
    double *B = (double *)malloc(N * sizeof(double));
    double *C = (double *)malloc(N * sizeof(double));

    // Initialize arrays A and B
    for (int i = 0; i < N; i++) {
        A[i] = 1.5;
        B[i] = 2.0;
    }

    double tstart = omp_get_wtime();

    // Strip Mining Implementation with OpenMP
    #pragma omp parallel
    {
        int total_threads = omp_get_num_threads();
        int tid = omp_get_thread_num();

        // Outer loop for strip mining: divide the total range into strips of size STRIP_SIZE
        #pragma omp for schedule(static)
        for (int s = 0; s < N; s += STRIP_SIZE) {
            int strip_end = s + STRIP_SIZE;
            if (strip_end > N) {
                strip_end = N;
            }

            // Inner loop processing the elements within the current strip
            for (int i = s; i < strip_end; i++) {
                C[i] = A[i] * B[i];
            }
        }
    }

    double tstop = omp_get_wtime();

    // Verify a sample of the results
    printf("Sample Results:\n");
    printf("C[0] = %.2f, C[500000] = %.2f, C[999999] = %.2f\n", C[0], C[500000], C[999999]);
    printf("Execution Time with Strip Mining and OpenMP: %.4f ms\n", (tstop - tstart) * 1000.0);

    // Free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}
