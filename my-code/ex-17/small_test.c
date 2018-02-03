#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int age = 20;
	FILE *file = fopen("info.txt", "a");
	int rc = fwrite(&age, sizeof(int), 1, file);
	if (rc != 1) {
		printf("Failed!\n");
		return 1;
	}
	rc = fflush(file);
	if (rc == -1) {
		printf("Failed flushing!\n");
		return 1;
	}
	int *age2 = malloc(sizeof(int));;
	FILE *file2 = fopen("info.txt", "r");
	rc = fread(age2, sizeof(int), 1, file2);
	if (rc != 1) {
		printf("Failed reading!\n");
		return 1;
	}
	printf("The read value is: %d.\n", *age2);
	free(age2);
	return 0;
}
