#include "cutting.h"

int main(int argc, char const *argv[])
{
	struct solution mySol;
	if (argc == 1)
		printf("Usage: cutting filename [-a]\n\n  -a\t\tDoes not read the value field, transforming the problem from\n\t\tmaximizing the value into maximizing the area utilized.\n");
	else {
		read(argv[1], &mySol);
		printf("  %d\n", solve(mySol));
		int i = 0;
		for (; i < mySol.size; i++)
			printf("%d ", mySol.optimum[i]);
		printf("\n");
		for (i = 0; i < mySol.size; i++)
			printf("%d ", mySol.patterns[i].max);
		printf("\n");
	}

	return 0;
}