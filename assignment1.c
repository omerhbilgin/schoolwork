// Programming assignment - 1

// This program calculates the square root of a given number by two methods,
// namely, the "interval search method" and the "newton-raphson" method,
// by using two functions: sqrt_interval_search and sqrt_newton_raphson,
// and prints the results to the screen. 
// And then the program runs these functions for every integer
// between 1 and 100,000. And prints to the screen, the execution
// time of each call. 
 
// Written by Omer Hamza Bilgin
// 7 November 2021
// Written in vim


#include <stdio.h>
#include <time.h>


// function prototypes
double sqrt_newton_raphson (double);
double sqrt_interval_search (double);


int main(void)
{

	double number;
	
	// for calculating the execution time of the processes
	clock_t t1, t2;
	int execution_time;

	
	// take user input and store it in "number"
	printf("Enter a number.\nIts square root will be calculated using the interval search method: ");
	scanf("%lf", &number);

	// print the number's square root using the interval search function
	printf("%lf\n\n", sqrt_interval_search(number));

	// take user input and store it in "number"
	printf("Enter a number.\nIts square root will be calculated using the newton-raphson method: ");
	scanf("%lf", &number);

	// print the number's square root suing the newton-raphson function
	printf("%lf\n\n", sqrt_newton_raphson(number));


	// this part prints the execution times of each function for all
	// integers between 1 and 100,000

	printf("interval_search\tnewton_raphson\n\n");

	for ( int i=1; i<=100000; i++ )
	{

		// this assignment is needed because the loop counter
		// here is an int, and the functions used below take
		// double as arguments.
		number = i;
		
		// start clock for the first funtion
		t1 = clock();
	
		// call the first function
		sqrt_interval_search(number);

		// end clock for the first function
		t2 = clock();

		// execution time of the first function	
		execution_time = (int) (t2 - t1);
	
		// print the execution time of the first function
		printf("%d\t", execution_time);

		// start clock for the second funciton
		t1 = clock();
		
		// call the second funciton
		sqrt_newton_raphson(number);
	
		// end clock for the second function
		t2 = clock();

		// execution time for the second function
		execution_time = (int) (t2 - t1);

		// print the execution time of the second function
		printf("%d\n", execution_time);
			
		
	}


	return 0;
}




// calculates the square root of a given number x
// using the newton-raphson method
double sqrt_newton_raphson (double x)
{
	
	// the root that the function is trying to calculate 
	double root = x;

	// the loop continues until root^2 is closer to x
	// than 0.0001.
	// in each iteration, the new root is calculated
	// according to the newton-raphson formula
	while ( (root*root - x) > 0.0001 )
	{
		root = root - ( (root*root - x) / (2.0 * root) );
	}

	return root;

}




// calculates the square root of a given number x
// using the interval search method.
// the interval starts as (0, x),
// and is divided to 10 equal parts in each iteration
// until the error margin becomes smaller than 0.0001
double sqrt_interval_search (double x)
{
	double beginning_of_interval = 0;
	double end_of_interval = x;
		
	double step = ( end_of_interval - beginning_of_interval ) / 10.0;


	// loop counter
	double i;

	double a0, a1;
	
	label:
	for ( i = beginning_of_interval; i < end_of_interval; i = i + step)
	{
		// the variables a0 and a1 are introduced for the intervals in each loop
		a0 = i;
		a1 = i+step;

		// f(a0) = a0*a0 - x
		// f(a1) = a1*a1 * x
		// if f(a0) * f(a1) is negative
		// it means that the root is in between
		// a0 and a1
		if ( (a0*a0 - x) * (a1*a1 - x) < 0 )
		{

			// if the difference between the two ends of the interval
			// is greater than 0.0001, then the process repeats.
			// the new interval now is (a0, a1), which is again divided
			// into 10 parts.
			if ( a1 - a0 > 0.0001 )
			{
				beginning_of_interval = a0;
				end_of_interval = a1;
				step = (end_of_interval - beginning_of_interval) / 10.0;
				goto label; 
			}
			
			// the function returns the middle value of the interval,
			// instead of a0 or a1;
			// hence even a better approximation to the real root.
			return (a0 + a1) / 2.0;
		}

	}	


	// if function fails
	return -1;

}
