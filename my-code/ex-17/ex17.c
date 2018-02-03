#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database {
	struct Address rows[MAX_ROWS];
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn) {
	if (conn) {
		if (conn->db) {
			free(conn->db);
		}
		if (conn->file) {
			fclose(conn->file);
		}
		free(conn);
	}
}

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

void Address_print(struct Address *addr) {
	printf("Address: %d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) {
		printf("Failed to load database!\n");
	}
}

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

void Database_write(struct Connection *conn) {
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) {
		die("Failed to write to file.", conn);
	}

	rc = fflush(conn->file);
	if (rc == -1) {
		die("Cannot flush the file.", conn);
	}
}

void Database_create(struct Connection *conn) {
	int i = 0;

	for(i = 0; i < MAX_ROWS; i++) {
		struct Address addr = {.id=i, .set=0};
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
	struct Address *addr = &conn->db->rows[id];
	if (addr->set) {
		die("Trying to set an already set address.", conn);
	}

	addr->set = 1;
	//BUG fix this later.
	char *res = strncpy(addr->name, name, MAX_DATA);
	if (!res) {
		die("Name copy failed.", conn);
	}

	res = strncpy(addr->email, email, MAX_DATA);
	if (!res) {
		die("Email copy failed.", conn);
	}
}

void Database_get(struct Connection *conn, int id) {
	struct Address *addr = &conn->db->rows[id];
	if (!addr->set) {
		die("Cannot get not set address, first set it", conn);
	} 
	Address_print(addr);
}

void Database_delete(struct Connection *conn, int id) {
	struct Address addr = {.id=id, .set=0};
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
	int i = 0;

	for(i = 0; i < MAX_ROWS; i++) {
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

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	if (argc > 3) {
		id = atoi(argv[3]);
	}
	if (id >= MAX_ROWS) {
		die("There are not that many records", conn);
	}

	switch(action) {
		case 'c':
			Database_create(conn);
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

