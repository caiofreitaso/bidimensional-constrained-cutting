#include "cutting.h"
#include <string.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	struct solution mySol;
	if (argc == 1)
		printf("Usage: cutting filename [-a]\n\n  -a\t\tDoes not read the value field, transforming the problem from\n\t\tmaximizing the value into maximizing the area utilized.\n");
	else {
		if (argc == 3) {
			printf("AREA MODE\n");
			read(argv[1], &mySol, !strncmp(argv[2],"-a",2));
		} else
			read(argv[1], &mySol, 0);
		
		printf("%dx%d (%d), %d patterns\n", mySol.sheet.x, mySol.sheet.y, mySol.sheet.x*mySol.sheet.y, mySol.size);
		time_t a,b;
		time(&a);
		int value = solve(mySol);
		time(&b);
		printf("TIME TAKEN:\t%1.20f\n", difftime(b,a));
		printf("MAXIMUM VALUE:\t%d\n", value);
		printf("SOLUTION:\t\t");
		int i = 0;
		for (; i < mySol.size; i++)
			printf("%d ", mySol.optimum[i]);
		printf("\nRESTRICTIONS:\t");
		for (i = 0; i < mySol.size; i++)
			printf("%d ", mySol.patterns[i].max);
		printf("\n");
		int area = 0;
		for (i = 0; i < mySol.size; i++)
			area += mySol.patterns[i].x * mySol.patterns[i].y * mySol.optimum[i];
		printf("AREA USED:\t\t%d (%d)\n", area, mySol.sheet.x*mySol.sheet.y - area);
	}

	return 0;
}