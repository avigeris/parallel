// seq.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include "pch.h"
// input: dimension, number threads and max iterations
// output: count iteration, work time and correctness of decision (check for adequacy set max iterations)

#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <time.h>
//#include <conio.h>

const double EPS = 0.000001;
void init(double *&, double *&, double *&, double *&, unsigned int &);
void compute(double *, double *, double *, double *, unsigned int &);
bool verify(double *, double *, double *, double *, unsigned int &);
void update_x(double *, double *, unsigned int &);
bool check(double *, double *, double *, unsigned int &);

void init(double *&a, double *&x, double *&x_new, double *&b, unsigned int &n) {
	a = (double *)malloc(sizeof(double) * n * n);
	x = (double *)malloc(sizeof(double) * n);
	x_new = (double *)malloc(sizeof(double) * n);
	b = (double *)malloc(sizeof(double) * n);

	// uncomment for use
	/*for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			scanf("%f", &a[i*n + j]);
		}
		scanf("%f", &b[i]);
		x[i] = b[i] / a[i*n + i];
	}*/

	//example of fill
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i == j) a[i*n + j] = 2.1 * (n - 1);
			else a[i*n + j] = 1.0;
		}

		if (i == n - 1) b[i] = 2.1 * (n - 1);
		else b[i] = 1.0;

		x[i] = b[i] / a[i*n + i];
		//x_new[i] = 0.0;
	}
}

void compute(double *a, double *x, double *x_new, double *b, unsigned int &n) {
	for (int i = 0; i < n; ++i) {
		x_new[i] = b[i];

		for (int j = 0; j < n; ++j) {
			if (i != j) x_new[i] -= a[i*n + j] * x[j];
		}
		x_new[i] /= a[i*n + i];
	}
}

bool verify(double *a, double *x, double *x_new, double *b, unsigned int &n) {
	double max_x_err = 0.0;
	for (int i = 0; i < n; ++i) {
		{
			if (max_x_err < fabs(x[i] - x_new[i]))
				max_x_err = fabs(x[i] - x_new[i]);
		}
	}

	double max_row_err = 0.0, sum = 0.0;
	for (int i = 0; i < n; ++i) {
		sum = b[i];
		for (int j = 0; j < n; ++j)
			sum -= a[i*n + j] * x_new[j];

		if (max_row_err < fabs(sum)) max_row_err = fabs(sum);
	}
	if (max_x_err < EPS && max_row_err < EPS)
		return true;

	return false;
}

void update_x(double *x, double *x_new, unsigned int &n) {
	for (int i = 0; i < n; ++i) x[i] = x_new[i];
}

bool check(double *a, double *x, double *b, unsigned int &n) {
	bool verdict = true;

	double sum = 0;
	for (int i = 0; i < n; ++i) {
		sum = 0;
		for (int j = 0; j < n; ++j) sum += a[i*n + j] * x[j];
		//std::cout << sum << " " << b[i] << std::endl;
		if (fabs(sum - b[i]) >= EPS) {
			verdict = false;
			//std::cout << i << ". " << sum << " " << b[i] << " " << abs(sum - b[i]) << std::endl;
		}
	}

	return verdict;
}

int main()
{
	//srand(time(NULL));

	//std::ios_base::sync_with_stdio(0);
	//std::cin.tie(0);


	unsigned int n, max_iter = 1000;
	scanf_s("%d", &n);

	double *a = { NULL };
	double *x = { NULL };
	double *x_new = { NULL };
	double *b = { NULL };

	init(a, x, x_new, b, n);

	int step = 0;
	double finish_time, start_time;

	time_t START = clock();
	//start_time = omp_get_wtime();
	do {
		update_x(x, x_new, n);
		compute(a, x, x_new, b, n);
		++step;
	} while (!verify(a, x, x_new, b, n) && step <= max_iter);
	//finish_time = omp_get_wtime();
	time_t FINISH = clock();

	printf("Count iterations:%d\n", step);
	if (check(a, x_new, b, n) == 1)
		printf("Ax==b\n");
	else
		printf("Ax!=b\n");
	printf("Time = %f\n", double(FINISH - START) / CLOCKS_PER_SEC);
	//printf("TimeOmp=%f\n", finish_time - start_time);

	//getch();
	return 0;
}