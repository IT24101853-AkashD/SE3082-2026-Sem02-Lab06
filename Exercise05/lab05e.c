#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int fib(int n) {
    int i, j;
    if (n < 2)
        return n;
    else {
        // Use tasks for parallelizing the recursive calls
        #pragma omp task shared(i) firstprivate(n)
        i = fib(n - 1);

        #pragma omp task shared(j) firstprivate(n)
        j = fib(n - 2);

        // Wait for both tasks to complete before summing
        #pragma omp taskwait
        
        return i + j;
    }
}

int main() {
    int n = 10; // You can change this to a higher number like 30 or 35 to see performance scaling
    int result;

    double tstart = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            result = fib(n);
        }
    }

    double tstop = omp_get_wtime();

    printf("Fibonacci of %d is %d\n", n, result);
    printf("Execution Time with OpenMP Tasks: %.4f ms\n", (tstop - tstart) * 1000.0);

    return 0;
}
