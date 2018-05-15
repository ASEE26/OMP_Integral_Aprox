
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <iomanip>
using std::setprecision;

void Usage(char *);
double f(double );   
double Trap(double , double , int , int );

int main(int argc, char* argv[]) {
   double  global_result = 0.0;  
   double  a, b;                 
   int     n;                    
   int     thread_count;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);

   cout<<"Ingrese a, b, y n \n";
   cin >> a >> b >> n;
  
   if (n % thread_count != 0) Usage(argv[0]);
#  pragma omp parallel num_threads(thread_count) 
   global_result = Trap(a, b, n, thread_count);

   cout<<"Con n = "<<n<<" trapezoides, estimamos \n";
   cout<<" La integral desde "<<a << "hasta "<<b<<" = "
       << setprecision(15)<<global_result<<endl;

   return 0;
}  /* main */

void Usage(char* prog_name) {

   fprintf(stderr, "Usar: %s <numero de hilos>\n", prog_name);
   fprintf(stderr, "   El numero de trapezoide debe ser dividible por la cantidad de hilos\n");
   fprintf(stderr, "   numero de hilos\n");
   exit(0);
}  /* Usage */


double f(double x) {
   double return_val;

   return_val = exp(-1*x*x);
   return return_val;
}  /* f */


void Trap(double a, double b, int n, double* global_result_p) {
   double  h, x, my_result;
   double  local_a, local_b;
   int  i, local_n;
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();

   h = (b-a)/n; 
   local_n = n/thread_count;  
   local_a = a + my_rank*local_n*h; 
   local_b = local_a + local_n*h; 
   my_result = (f(local_a) + f(local_b))/2.0; 
   for (i = 1; i <= local_n-1; i++) {
     x = local_a + i*h;
     my_result += f(x);
   }
   my_result = my_result*h; 

#  pragma omp critical 
   *global_result_p += my_result; 
}  
