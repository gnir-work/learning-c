#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
	char *name;
	int age;
	int height;
	int weight;
};

/**
 * Creates a person struct and returns a pointer to it.
 */
struct Person *Person_create(char *name, int age, int height, int weight) {
	struct Person *who = malloc(sizeof(struct Person));
	assert(who != NULL);

	who->name = strdup(name);
	who->age = age;
	who->height = height;
	who->weight = weight;

	return who;
}

/**
 * Frees the memory allocated by the struct of type Person which the
 * pointer who points to.
 * NOTE: we need to free first the the memory which Person->name points to.
 */
void Person_destroy(struct Person *who) {
	assert(who != NULL);

	free(who->name);
	free(who);
}

/**
 * Prints out the properties of the struct Person.
 */
void Person_print(struct Person *who) {
	printf("name: %s\n", who->name);
	printf("\tAge: %d\n", who->age);
	printf("\tHeight: %d\n", who->height);
	printf("\tWeight: %d\n", who->weight);
	printf("\n");
}

int main(int argc, char *argv[]) {
	//Create two people
	struct Person *nir = Person_create("nir", 20, 87, 89);
	struct Person *oshrit = Person_create("oshrit", 19, 75, 71);

	//Print them out
	printf("the size of a Person is: %ld\n", sizeof(struct Person));
	
	printf("Nir is at %p in my memory.\n", nir);
	Person_print(nir);

	printf("Oshrit is at %p in my memory.\n", oshrit);
	Person_print(oshrit);

	//Make them old
	nir->age += 20;
	nir->weight += 15;
	nir->height -= 2;

	oshrit->age += 20;
	oshrit->weight += 1;
	oshrit->height -= 1;

	//Print them out again.
	printf("Nir is at %p in my memory.\n", nir);
	Person_print(nir);

	printf("Oshrit is at %p in my memory.\n", oshrit);
	Person_print(oshrit);	

	//Destroy them!
	Person_destroy(nir);
	Person_destroy(oshrit);

	return 0;
}

