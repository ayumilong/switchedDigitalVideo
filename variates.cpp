/*------------------------------------------------------------------------
 *  Module: Variates
 * 
 *  Function:  provides several routines to generator random variates
 *
 *  The zipf RV code is from  Kenneth J. Christensen (University of South Florida )
-------------------------------------------------------------------------*/
#include "variates.h"
#include <iostream>


/***************************************************************
*  Routine:  int zipfRN(double shape, int n)
*
*  Inputs:
*    double shape:  power law shape parameter
*    int n :   the number of items in the list 
* 
*  Outputs:
*    Returns a variate (a random sample)
*
**************************************************************/
int zipfRN(double shape, int n)
{
  static int first = TRUE;      // Static first time flag
  static double c = 0;          // Normalization constant
  double z;                     // Uniform random number (0 < z < 1)
  double sum_prob;              // Sum of probabilities
  double zipf_value;            // Computed exponential value to be returned
  int    i;                     // Loop counter

  // Compute normalization constant on first call only
  if (first == TRUE)
  {
    //printf("zipfRN: First invocation,  n:%d \n",n);

    for (i=1; i<=n; i++)
      c = c + (1.0 / pow((double) i, shape));
    c = 1.0 / c;
    first = FALSE;
    // set the RNG seed to a default value just the first time!!
	srand(time(NULL));
    rand_val(random());
  }

  // Pull a uniform random number (0 < z < 1)
  do
  {
    z = rand_val(0);
  }
  while ((z == 0) || (z == 1));

  // Map z to the value
  sum_prob = 0;
  for (i=1; i<=n; i++)
  {
    sum_prob = sum_prob + c / pow((double) i, shape);
    if (sum_prob >= z)
    {
      zipf_value = i;
      break;
    }
  }

  // Assert that zipf_value is between 1 and N
  assert((zipf_value >=1) && (zipf_value <= n));

  return(zipf_value);
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double rand_val(int seed)
{
  const long  a =      16807;  // Multiplier
  const long  m = 2147483647;  // Modulus
  const long  q =     127773;  // m div a
  const long  r =       2836;  // m mod a
  static long x;               // Random int value
  long        x_div_q;         // x divided by q
  long        x_mod_q;         // x modulo q
  long        x_new;           // New x value

  // Set the seed if argument is non-zero and then return zero
  if (seed > 0)
  {
    x = seed;
    return(0.0);
  }

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return((double) x / m);
}

/***************************************************************
*  Routine:  double exponentialRN(double meanRV)
*
*  Inputs:
*    double meanRV:  The requested mean of the Random Variable
* 
*  Outputs:
*    Returns a variate (a random sample)
*
**************************************************************/
double exponentialRN(double meanRV)
{
  double uniformRN = 1.0;
  double expoRN = - 1.0;

  static int first = TRUE;      // Static first time flag

  // Compute normalization constant on first call only
  if (first == TRUE)
  {
    //printf("exponentialRN: First invocation,  mean:%3.3f \n",meanRV);
    first = FALSE;
    // set the RNG seed to a default value just the first time!!
	srand(time(NULL));
    rand_val(random());
  }

  uniformRN = rand_val(0);
  expoRN = -(1/meanRV)*log(uniformRN);

  return expoRN;
}
