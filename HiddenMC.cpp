// These functions are found below.
void    GetData ();
void    Report ();
double *AllocateMemory();

// These global variables are used to reduce function arguments.
double *R, *R2, *sigma, *EWMA, *s, *p, *ptilde, *g;

#include "Functions.h"
#include <cmath>
#include <iostream>

int main () {
   
   double alpha, Z, U, sig, khat, map, p_t; 
   // Read in the time series data.
   GetData ();

   //initializes alpha and sigma values that are given 
   alpha = 0.038; 
   sig = 0.5; 

   //p_0 = 1 because random walk starts at 0 (W_0 = 0)
   p[0] = 1; 

   //loops through elapsed time in days  
   for (int t = 1; t <=1258; t++){

      //holds values for MAP estimation
      khat = 0.0; 
      map = 0.0;
      //Resets Z to zero 
      Z = 0.0;

      //uses MAP estimation to find the k value 
      //for which the conditional probability of the walk
      //at state k at time t is maximized
      
      //loop cycles through the states k at time t in increments of 2 
      for (int k = -t; k<= t; k+=2){
         p_t = (p[k+1] +p[k-1]) /2.0;

         if (p_t > map){
            khat = k;
            map = p_t;   }
      
         //caculates s_k - the daily volatility when MC is in state k
         s[k] = sig*exp(alpha*k); 
         //calculates g_k - the numerator for Bayesian updating
         g[k] = 1/(sqrt(2*M_PI)*s[k]) * exp(-R2[t]/(2*s[k]*s[k])) *p_t;

         //Z = sum of g[k]'s at time t 
         Z += g[k];
      }

      //sigma undergoes exponential random walk 
      //sigma_t = sigma*exp(alpha * W_t) 
      sigma[t] = sig*exp(alpha*khat);
      

      //updates values for p using the posterior ptilde
      for (int k = -t; k<=t; k+=2){
         ptilde[k] = g[k]/Z;
         p[k] = ptilde[k];
      }

   }



   // Create TeX files for viewing results.
   Report ();
   Exit();


}


////////////////////////////////////////////////////////////////////////////////
// Read in a daily time series of stock price returns R[t] 1 <= t <= 1258.
// Allocate array space for the model
////////////////////////////////////////////////////////////////////////////////
void GetData () {

   int t, n;
   char input[100];
   FILE *fp;

   fp = fopen ("XOM5YrsDaily.txt", "r");

   // Read in the file description.
   fgets (input, 99, fp);

   // Allocate memory for the data, initialized to 0.
   R     = List (1258);
   R2    = List (1258);
   sigma = List (1258);
   EWMA  = List (1258);

   // Allocate necessary memory for the HMC model.
   s = AllocateMemory ();
   p = AllocateMemory ();
   ptilde = AllocateMemory ();
   g = AllocateMemory ();

   // Now read in the XOM return data (in percent).
   for (t = 1; t <= 1258; t++) {
      fgets (input, 99, fp);
      sscanf (input, "%lf", R+t);
      R2[t] = R[t] * R[t];
   }
   fclose (fp);

   // Now read in the EWMA data (for purposes of the scatter plot).
   fp = fopen ("EWMA.txt", "r");
   for (t = 51; t <= 1258; t++) {
      fgets (input, 99, fp);
      sscanf (input, "%d %lf", &n, EWMA+t);
   }
   fclose (fp);

   return;

}

////////////////////////////////////////////////////////////////////////////////
// Generate some output files.
////////////////////////////////////////////////////////////////////////////////
void Report () {

   int t;
   double annualizedVol, Z;
   FILE *fp, *fp2;

   // First create a time series of HMC estimated annualized volatility,
   //    and standardized XOM return.
   fp  = fopen ("HMC.txt", "w");
   fp2 = fopen ("StandardizedXOM.txt", "w");

   // Start at day 51.
   for (t = 51; t <= 1258; t++) {

      annualizedVol = sqrt(252.0) * sigma[t];
      fprintf (fp, "%4d %10.2f\n", t, annualizedVol);

      // Standardize the data.
      Z = R[t] / sigma[t];               

      // Add the standardized return to the histogram with 20 buckets.
      NormalHistogram (Z, 20, 0);

      // Now put it into the time series file
      fprintf (fp2, "%4d  %6.3f\n", t, Z);

   }

   fclose (fp);
   fclose (fp2);

   // Create histogram TeX file.
   NormalHistogram (0, 20, 1);

   // Now create the scatter plot data file.
   fp = fopen ("ScatterData.txt", "w");
   for (t = 51; t <= 1258; t++) {
      fprintf (fp, "%6.2f  %6.2f\n", EWMA[t], sqrt(252.0) * sigma[t]);
   }
   fclose (fp);

   return;

}

////////////////////////////////////////////////////////////////////////////////
// Allocate memory for an array with indices from -1260 to +1260.
// This is a little more than needed.
////////////////////////////////////////////////////////////////////////////////
double *AllocateMemory () {

   double *x;

   x = (double *) calloc (2*1260 + 1, sizeof (double));

   x += 1260;

   return x;

}





