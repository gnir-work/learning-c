#include <stdio.h>

int main(int argc, char* argv[]) {
	int i = 0;

	if (argc == 1) {
		printf("You only have one argument. You Suck!\n");
	} else if (argc > 1 && argc < 4) {
		printf("Here are your arguments:\n");

		for (i = 0; i < argc; i++) {
			printf("%s, ", argv[i]);
		}
		printf("\n");
	} else {
		printf("You to many argument. You Suck!\n");
	}

	return 0;
}
