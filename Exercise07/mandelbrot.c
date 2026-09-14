#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int width = 2000;
    int height = 2000;
    int max_iterations = 2000;
    
    double real_min = -2.0;
    double real_max = 0.5;
    double imag_min = -1.25;
    double imag_max = 1.25;

    double dx = (real_max - real_min) / (double)width;
    double dy = (imag_max - imag_min) / (double)height;

    int thread_counts[] = {1, 2, 3, 4};
    int num_tests = 4;

    printf("=== Mandelbrot Set Area Estimation (Lab 05 - Exercise 7) ===\n");

    for (int t_idx = 0; t_idx < num_tests; t_idx++) {
        int threads = thread_counts[t_idx];
        omp_set_num_threads(threads);

        long inside_count = 0;
        double tstart, tstop, tcalc;

        // Timing code as specified in instructions
        tstart = omp_get_wtime();

        #pragma omp parallel for reduction(+:inside_count) schedule(static)
        for (int i = 0; i < height; i++) {
            double c_imag = imag_min + i * dy;
            for (int j = 0; j < width; j++) {
                double c_real = real_min + j * dx;
                
                double z_real = c_real;
                double z_imag = c_imag;
                int inside = 1;

                for (int n = 0; n < max_iterations; n++) {
                    double z_real2 = z_real * z_real;
                    double z_imag2 = z_imag * z_imag;

                    if (z_real2 + z_imag2 > 4.0) {
                        inside = 0;
                        break;
                    }

                    double z_real_temp = z_real2 - z_imag2 + c_real;
                    z_imag = 2.0 * z_real * z_imag + c_imag;
                    z_real = z_real_temp;
                }

                if (inside) {
                    inside_count++;
                }
            }
        }

        tstop = omp_get_wtime();
        tcalc = (tstop - tstart) * 1000.0; // time in milliseconds

        double total_box_area = (real_max - real_min) * (imag_max - imag_min);
        double area = total_box_area * ((double)inside_count / (double)(width * height));

        printf("Threads: %d | Estimated Area: %.6f | Time: %.2f ms\n", 
               threads, area, tcalc);
    }

    return 0;
}
