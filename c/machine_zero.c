/* machine zero */
#include <stdio.h>

int main()
{
  float f, ft;
  double d, dt;
  long double ld, ldt;
  
  printf("Machine zero\n");
  printf("Enter start value\n");
  scanf ("%e", &f);
  ft = d = dt = ld = ldt = f;
  
  int i = 0, j = 0 , k = 0;
  
  do
  {
    i++;
    ft /= 2;
    if (0 + ft == 0)
      printf("Float\t\t\t: %e\nNumber of iterations\t: %d\n", f, i);
    f = ft;
  }
  while (0 + ft != 0);
  
  do
  {
    j++;
    dt /= 2;
    if (0 + dt == 0)
      printf("Double\t\t\t: %e\nNumber of iterations\t: %d\n", d, j);
    d = dt;
  }
  while (0 + dt != 0);
  
  do
  {
    k++;
    ldt /= 2;
    if (0 +ldt == 0)
      printf("Long double\t\t: %e\nNumber of iterations\t: %d\n", ld, k);
    ld = ldt;
  }
  while (0 + ldt != 0);
  
  return 0;
}
