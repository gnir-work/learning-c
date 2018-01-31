#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("ERROR: you must pass arguments to the script!\n");
		return 1;
	}
	int distance_between_upper_and_lower = 32;
	int j = 0;
	int i = 0;
	for(j = 1; j < argc; j++) {
		printf("Working on word '%s'.\n", argv[j]);
		char letter = argv[j][0];
		for (i = 0; argv[j][i] != '\0'; i++, letter = argv[1][i]) {
			printf("%d", i);
			if (letter < 97) {
				letter += distance_between_upper_and_lower;
			}
			printf("------%c-----", letter);
			switch(letter) {
				case 'a':
					printf("%d: 'A'\n", i);
					break;
				case 'e':
					printf("%d: 'E'\n", i);
					break;
				case 'i':
					printf("%d: 'I'\n", i);
					break;
				case 'o':
					printf("%d: 'O'\n", i);
					break;
				case 'u':
					printf("%d: 'U'\n", i);	
					break;
				case 'y':
					if (i > 2) {
						printf("%d: 'Y'\n", i);
					}
					break;
				default:
					printf("%d: %c is not a vowel\n", i, letter);
			}
		}
	}

	return 0;
}
