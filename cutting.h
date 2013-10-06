#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

struct pattern {
	int x;
	int y;
	int value;
	unsigned max;
	unsigned quantity;
};

struct plane {
	int x;
	int y;
};

struct answer {
	int index;
	struct answer* parent;
};

struct solution {
	struct plane sheet;
	struct pattern *patterns;
	int *optimum;
	int size;
};

int cut_a(struct plane sheet, struct pattern type, struct plane newPieces[]) {
	if (sheet.y >= type.y && sheet.x >= type.x) {
		newPieces[0].x = sheet.x;
		newPieces[0].y = sheet.y - type.y;
		
		newPieces[1].x = sheet.x - type.x;
		newPieces[1].y = type.y;
		return -1;
	} else
		return 0;
}
int cut_b(struct plane sheet, struct pattern type, struct plane newPieces[]) {
	if (sheet.x >= type.y && sheet.y >= type.x) {
		newPieces[0].x = sheet.x;
		newPieces[0].y = sheet.y - type.x;
		
		newPieces[1].x = sheet.x - type.y;
		newPieces[1].y = type.x;
		return -1;
	} else
		return 0;
}

int run(struct plane sheet, struct pattern types[], int types_size, int solution[]) {

	int i = 0;
	
	static int depth = 0;
	int temp_sol[types_size];
	for(; i < types_size; i++)
		temp_sol[i] = solution[i];

	int optimum = 0;
	struct plane results_a[2], results_b[2];
	int j;
	for (i = 0; i < types_size; i++)
		if (types[i].max > solution[i]) {
			depth++;
			int sol_a[types_size], sol_b[types_size];

			if (cut_a(sheet,types[i],results_a)) {
				for (j = 0; j < types_size; j++)
					sol_a[j] = solution[j];
				sol_a[i]++;

				int a = run(results_a[0],types,types_size, sol_a);
				int b = run(results_a[1],types,types_size, sol_a);
				
				if (types[i].value + a + b > optimum) {
					optimum = types[i].value + a + b;
					for (j = 0; j < types_size; j++)
						temp_sol[j] = sol_a[j];
				}
			}

			if (cut_b(sheet,types[i],results_b)) {
				for (j = 0; j < types_size; j++)
					sol_b[j] = solution[j];
				sol_b[i]++;

				int a = run(results_b[0],types,types_size, sol_b);
				int b = run(results_b[1],types,types_size, sol_b);
				
				if (types[i].value + a + b > optimum) {
					optimum = types[i].value + a + b;
					for (j = 0; j < types_size; j++)
						temp_sol[j] = sol_b[j];
				}
			}
			
			for(j = 0; j < types_size; j++)
				solution[j] = temp_sol[j];

			depth--;
		}

	return optimum;
}

int solve(struct solution problem) {
	printf("%dx%d, %d patterns\n", problem.sheet.x, problem.sheet.y, problem.size);
	return run(problem.sheet, problem.patterns, problem.size, problem.optimum);
}


int read(char const* file, struct solution* output) {
	FILE *fpointer = 0;
	if ((fpointer = fopen(file, "r"))) {
		fseek(fpointer, 0, SEEK_END);
		long size = ftell(fpointer);
		rewind(fpointer);
		
		char *buffer = 0;
		if ((buffer = (char*) malloc(sizeof(char) * size))) {
			if (fread(buffer,1,size,fpointer) == size) {
				fclose(fpointer);

				output->sheet.x = atoi(buffer);
				while(*buffer != ' ')
					buffer++;
				buffer++;
				output->sheet.y = atoi(buffer);
				while(*buffer != ' ')
					buffer++;
				buffer++;
				output->size = atoi(buffer);
				output->patterns = (struct pattern*) malloc(sizeof(struct pattern) * output->size);
				output->optimum = (int*) malloc(sizeof(int) * output->size);
				int i = 0;
				for(; i < output->size; i++)
					output->optimum[i] = 0;
				while(*buffer != '\n')
					buffer++;
				buffer++;
				for (i = 0; i < output->size; i++) {
					output->patterns[i].x = atoi(buffer);
					while(*buffer != ' ')
						buffer++;
					buffer++;
					output->patterns[i].y = atoi(buffer);
					while(*buffer != ' ')
						buffer++;
					buffer++;
					//output->patterns[i].value = output->patterns[i].x*output->patterns[i].y;
					output->patterns[i].value = atoi(buffer);
					while(*buffer != ' ')
						buffer++;
					buffer++;
					output->patterns[i].max = atoi(buffer);
					while(*buffer != '\n')
						buffer++;
					if (*buffer)
						buffer++;
				}
				return 0;
			} else {
				fclose(fpointer);
				fprintf(stderr, "Reading error: %s\n", strerror(errno));
				exit(1);
			}
		} else {
			fclose(fpointer);
			fprintf(stderr, "Memory error: %s\n", strerror(errno));
			exit(1);
		}
	} else {
		fprintf(stderr, "File error: %s\n", strerror(errno));
		exit(1);
	}
	return 1;
}