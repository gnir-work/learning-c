#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
	struct Address *rows;
	int max_data;
	int max_rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

/**
 * Close the connection to the database.
 * Which in practice means freeing the space allocated
 * for The Database struct and clossing the db file.
 */
void Database_close(struct Connection *conn) {
	if (conn) {
		if (conn->db) {
			if (conn->db->rows) {
				free(conn->db->rows);
			}
			free(conn->db);
		}
		if (conn->file) {
			fclose(conn->file);
		}
		free(conn);
	}
}

/**
 * Close the database and terminate the function with
 * an error message.
 */
void die(char *message, struct Connection *conn) {
	if(conn) {
		Database_close(conn);
	}
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s!\n", message);
	}
	
	exit(1);
}

/**
 * Print out the address struct.
 */
void Address_print(struct Address *addr) {
	printf("Address: %d %s %s\n", addr->id, addr->name, addr->email);
}

/**
 * Load the database from the file
 */
void Database_load(struct Connection *conn) {
	unsigned long db_size = conn->db->max_rows * sizeof(struct Address) + 2 * sizeof(int);
	int rc = fread(conn->db, db_size, 1, conn->file);
	if (rc != 1) {
		printf("Failed to load database!\n");
	}
}

/**
 * Malloc some space for the connection struct, the Database struct 
 * and open the file used for saving the db.
 */
struct Connection *Database_open(const char *filename, char mode) {
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn) {
		die("Memory Error", conn);
	}

	conn->db = malloc(sizeof(struct Database));
	if (!conn->db) {
		die("Memory Error", conn);
	}

	if (mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");

		if (conn->file) {
			Database_load(conn);
		}
	}

	if (!conn->file) {
		die("Failed to open the file", conn);
	}

	return conn;
}

/**
 * Write the current state of the Database struct to the file
 */
void Database_write(struct Connection *conn) {
	rewind(conn->file);
	unsigned long db_size = conn->db->max_rows * sizeof(struct Address) + 2 * sizeof(int);
	int rc = fwrite(conn->db, db_size, 1, conn->file);
	if (rc != 1) {
		die("Failed to write to file.", conn);
	}

	rc = fflush(conn->file);
	if (rc == -1) {
		die("Cannot flush the file.", conn);
	}
}

/**
 * Initialize the db with empty rows.
 */
void Database_create(struct Connection *conn, int max_rows, int max_data) {
	//Set the max_rows and max_data properties of struct Database.	
	conn->db->max_rows = max_rows;
	conn->db->max_data = max_data;

	//Malloc memory for the rows array inside of the Database struct.
	conn->db->rows = calloc(conn->db->max_rows, sizeof(struct Address));
	if (!conn->db->rows) {
		die("Couldn't alloc space for the rows of the db.", conn);
	}
	
	int i = 0;

	for(i = 0; i < conn->db->max_rows; i++) {
		struct Address addr = {.id=i, .set=0};
		conn->db->rows[i] = addr;
	}
}

/**
 * Set a specific row in the db to the given name and email
 * NOTE: The updated db isn't saved automatically to the file,
 * rather Database_write should be called.
 */
void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
	if (id >= conn->db->max_rows) {
		die("There are not that many records", conn);
	}

	struct Address *addr = &conn->db->rows[id];
	if (addr->set) {
		die("Trying to set an already set address.", conn);
	}

	addr->set = 1;
	char *res = strncpy(addr->name, name, conn->db->max_data);
	if (!res) {
		die("Name copy failed.", conn);
	} else {
		res[conn->db->max_data - 1] = '\0';
	}

	res = strncpy(addr->email, email, conn->db->max_data);
	if (!res) {
		die("Email copy failed.", conn);
	} else {
		res[conn->db->max_data - 1] = '\0';
	}
}

/**
 * Print out a row in the database.
 */
void Database_get(struct Connection *conn, int id) {
	if (id >= conn->db->max_rows) {
		die("There are not that many records", conn);
	}
	
	struct Address *addr = &conn->db->rows[id];
	if (!addr->set) {
		die("Cannot get not set address, first set it", conn);
	} 
	Address_print(addr);
}

/**
 * Delete a row from the database
 */
void Database_delete(struct Connection *conn, int id) {
	if (id >= conn->db->max_rows) {
		die("There are not that many records", conn);
 	}
	struct Address addr = {.id=id, .set=0};
	conn->db->rows[id] = addr;
}

/**
 * Print out all of the rows that have been set in the database.
 */
void Database_list(struct Connection *conn) {
	int i = 0;

	for(i = 0; i < conn->db->max_rows; i++) {
		struct Address *addr = &conn->db->rows[i];

		if (addr->set) {
			Address_print(addr);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		die("USAGE: ./ex17 <dbfile> <action> [action params]", NULL);
	}
	
	//Retrieved the name of the db from the arguments
	char *filename = argv[1];
	//Retrieved the action from the arguments.
	char action = argv[2][0];
	//Created the connection struct.
	struct Connection *conn = Database_open(filename, action);

	int id = 0;

	if (argc > 3) {
		//Retrieved the id from the arguments, if it was given.
		id = atoi(argv[3]);
	}
	
	switch(action) {
		case 'c':
			if (argc != 5) {
				die("Need the number of rows and size of data to create", conn);
			}
			int max_rows = atoi(argv[3]);
			int max_data = atoi(argv[4]);
			Database_create(conn, max_rows, max_data);
			Database_write(conn);
			break;
		case 'g':
			if (argc != 4) {
				die("Need an id to get", conn);
			}

			Database_get(conn, id);
			break;
		case 's':
			if (argc != 6) {
				die("Need id, name, email to set", conn);
			}

			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if (argc != 4) {
				die("Need id to delete", conn);
			}

			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die("Invalid action: c=create, g=get, s=set, d=delete, l=list.\n", conn);
	}

	Database_close(conn);
	return 0;
}


