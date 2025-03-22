#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

// struct timeval {
//     time_t      tv_sec;     // seconds
//     suseconds_t tv_usec;    // microseconds
// };//calcul diff time in function 

// int gettimeofday(struct timeval *tv, struct timezone *tz);

void calcul()
{
	struct timeval	start ,end;
	long second, microsecond;
	double	elapsed_seconds ;
	long total;

	gettimeofday(&start,NULL);
	for(int i = 0; i < 1000000 ; i++){}
	
	gettimeofday(&end,NULL);
	
	second = end.tv_sec - start.tv_sec;
	microsecond  = end.tv_usec - start.tv_usec;

		while(microsecond < 0 )
		{
		second--;
		microsecond += 1000000;
	}
	printf("seconds %ld , microsecond %ld\n",second,microsecond);
	total = second * 1000000 + microsecond;
    printf("diff_totla , %ld  \n", total);
	    elapsed_seconds = (double)second + (double)microsecond /  1000000;
	//printf( " %.6f \n", elapsed_seconds);	
	   int minutes = (int)(elapsed_seconds / 60);
    int seconds = (int)(elapsed_seconds) % 60;

    printf("%02d:%02d\n", minutes, seconds); 
}



void time_in_func()
{
	
    struct timeval time;

    gettimeofday(&time, NULL);
    printf("%ld seconds\n", time.tv_sec);
    printf("%ld microseconds\n", time.tv_usec);

    printf("\n%ld years passed since 1970\n", time.tv_sec / 60 / 60 / 24 / 365);
}
void time_diff()
{
    struct timeval start_time, end_time;
    long duration;

    start_time.tv_sec = 10;
    start_time.tv_usec = 500000;

    end_time.tv_sec = 12;
    end_time.tv_usec = 200000;

    duration = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);

    printf("\n%ld micro\n ", duration);
    printf("\n%.6f second \n", (double)duration / 1000000);

}

int main(int ac , char **av)
{
	calcul();
	time_in_func();
	time_diff();

}
