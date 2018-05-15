
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
//#  pragma omp parallel num_threads(thread_count) 
   global_result = Trap(a, b, n, thread_count);

   cout<<"Con n = "<<n<<" trapezoides, estimamos \n";
   //printf("Con n = %d trapezoide, estimamos\n", n);
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


double Trap(double a, double b, int n, int thread_count) {
   double  h, approx;
   int  i;

   h = (b-a)/n; 
   approx = (f(a) + f(b))/2.0; 
# pragma omp parallel for num_threads(thread_count) \
      reduction(+: approx)
   for (i = 1; i <= n-1; i++)
     approx += f(a + i*h);
   approx = h*approx; 

   return approx;
} 
