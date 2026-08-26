#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Address
struct AddressMeta {
  size_t id;
  bool set;
  size_t name_size;
  size_t email_size;
};

struct Address {
  struct AddressMeta meta;
  char *name;
  char *email;
};

int Address_init(struct Address *address, struct AddressMeta meta);
void Address_deinit(struct Address *address);
int Address_to_file(const struct Address *address, FILE *file);
int Address_from_file(struct Address *address, FILE *file);
void Address_print(const struct Address *addr);

int Address_init(struct Address *address, struct AddressMeta meta) {
  if (address == NULL) {
    return -1;
  }

  address->meta = meta;
  address->name = NULL;
  address->email = NULL;

  address->name = calloc(1, meta.name_size);
  if (address->name == NULL) {
    return -1;
  }

  address->email = calloc(1, meta.email_size);
  if (address->email == NULL) {
    free(address->name);
    address->name = NULL;
    return -1;
  }

  return 0;
}

void Address_deinit(struct Address *address) {
  if (address != NULL) {
    free(address->name);
    free(address->email);
    address->name = NULL;
    address->email = NULL;
  }
}

int Address_to_file(const struct Address *address, FILE *file) {
  if (address == NULL || file == NULL) {
    return -1;
  }

  struct AddressMeta meta = address->meta;
  if (fwrite(&meta, sizeof(struct AddressMeta), 1, file) != 1) {
    return -1;
  }
  if (fwrite(address->name, meta.name_size, 1, file) != 1) {
    return -1;
  }
  if (fwrite(address->email, meta.email_size, 1, file) != 1) {
    return -1;
  }

  return 0;
}

/** address 传入时需要满足未初始化状态 */
int Address_from_file(struct Address *address, FILE *file) {
  if (address == NULL || file == NULL) {
    return -1;
  }

  struct AddressMeta meta;
  if (fread(&meta, sizeof(struct AddressMeta), 1, file) != 1) {
    return -1;
  }
  if (Address_init(address, meta) != 0) {
    return -1;
  }
  if (fread(address->name, meta.name_size, 1, file) != 1) {
    goto read_fail;
  }
  if (fread(address->email, meta.email_size, 1, file) != 1) {
    goto read_fail;
  }
  return 0;

read_fail:
  Address_deinit(address);
  return -1;
}

void Address_print(const struct Address *addr) {
  printf("id=%zu, name=%s email=%s\n", addr->meta.id, addr->name, addr->email);
}

/// Database
struct DatabaseMeta {
  size_t max_data_rows;
};

struct Database {
  struct DatabaseMeta meta;
  struct Address *rows;
};

struct Connection {
  FILE *file;
  struct Database *db;
};

struct Connection *Database_open(const char *filename, char mode);
void Database_create(struct Connection *conn, struct DatabaseMeta);
void Database_load(struct Connection *conn);
void Database_write(struct Connection *conn);
void Database_close(struct Connection *conn);
void Database_set(struct Connection *conn, size_t id, const char *name,
                  const char *email);
void Database_get(struct Connection *conn, size_t id);
void Database_delete(struct Connection *conn, size_t id);
void Database_list(struct Connection *conn);
void Database_search(struct Connection *conn, const char *field,
                     const char *pattern);
void die(const char *message, struct Connection *conn);

/**
 * Print the message, close the connection (can be NULL) and exit with 1.
 */
void die(const char *message, struct Connection *conn) {
  if (errno) {
    fprintf(stderr, "%s: ", strerror(errno));
  }
  fprintf(stderr, "%s\n", message);

  Database_close(conn);
  exit(1);
}

struct Connection *Database_open(const char *filename, char mode) {
  struct Connection *conn = calloc(1, sizeof(struct Connection));
  if (!conn)
    die("Database_open: Memory error", conn);

  conn->db = calloc(1, sizeof(struct Database));
  if (!conn->db)
    die("Database_open: Memory error", conn);

  if (mode == 'c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");
  }

  if (!conn->file)
    die("Database_open: Failed to open the file", conn);

  return conn;
}

void Database_create(struct Connection *conn, struct DatabaseMeta dbm) {
  conn->db->meta = dbm;
  conn->db->rows = calloc(dbm.max_data_rows, sizeof(struct Address));
  if (!conn->db->rows)
    die("Database_create: Memory error", conn);

  for (size_t i = 0; i < dbm.max_data_rows; i++) {
    // 为了序列化正常工作，我们需要保证字段长度不为 0。4B 对齐就很不错。
    struct AddressMeta addrm = {0};
    // 处理 padding，先初始化再赋值。
    addrm = (struct AddressMeta){i, false, 4, 4};
    if (Address_init(&conn->db->rows[i], addrm) != 0)
      die("Database_create: Memory error", conn);
  }
}

void Database_write(struct Connection *conn) {
  rewind(conn->file);

  struct DatabaseMeta dbm = conn->db->meta;
  if (fwrite(&dbm, sizeof(struct DatabaseMeta), 1, conn->file) != 1)
    die("Database_write: Failed to write database.", conn);

  for (size_t i = 0; i < dbm.max_data_rows; ++i) {
    struct Address *addr = conn->db->rows + i;
    if (Address_to_file(addr, conn->file) != 0)
      die("Database_write: Failed to write database.", conn);
  }

  if (fflush(conn->file) == -1)
    die("Database_write: Canot flush database", conn);
}

void Database_load(struct Connection *conn) {
  struct DatabaseMeta dbm;
  if (fread(&dbm, sizeof(struct DatabaseMeta), 1, conn->file) != 1)
    die("Database_load: Failed to load database.", conn);
  conn->db->meta = dbm;
  conn->db->rows = calloc(dbm.max_data_rows, sizeof(struct Address));
  if (!conn->db->rows)
    die("Database_load: Memory error", conn);

  for (size_t i = 0; i < dbm.max_data_rows; ++i) {
    struct Address *row = conn->db->rows + i;
    if (Address_from_file(row, conn->file) != 0)
      die("Database_load: Failed to load database.", conn);
  }
}

void Database_close(struct Connection *conn) {
  if (conn) {
    if (conn->file)
      fclose(conn->file);
    if (conn->db) {
      if (conn->db->rows) {
        struct DatabaseMeta meta = conn->db->meta;
        for (size_t i = 0; i < meta.max_data_rows; ++i) {
          Address_deinit(&conn->db->rows[i]);
        }
        free(conn->db->rows);
      }
      free(conn->db);
    }
    free(conn);
  }
}

void Database_set(struct Connection *conn, size_t id, const char *name,
                  const char *email) {
  struct Address *addr = &conn->db->rows[id];
  if (addr->meta.set)
    die("Database_set: Already set, delete it first", conn);

  char *new_name = strdup(name);
  if (!new_name)
    die("Database_set: Name copy failed", conn);

  char *new_email = strdup(email);
  if (!new_email) {
    free(new_name);
    die("Database_set: Email copy failed", conn);
  }

  free(addr->name);
  free(addr->email);
  addr->name = new_name;
  addr->email = new_email;
  addr->meta.id = id;
  addr->meta.set = true;
  addr->meta.name_size = strlen(name) + 1;
  addr->meta.email_size = strlen(email) + 1;
}

void Database_get(struct Connection *conn, size_t id) {
  struct Address *addr = &conn->db->rows[id];
  if (addr->meta.set) {
    Address_print(addr);
  } else {
    die("Database_get: ID is not set", conn);
  }
}

void Database_delete(struct Connection *conn, size_t id) {
  struct Address *addr = conn->db->rows + id;
  addr->meta.set = false;
}

void Database_list(struct Connection *conn) {
  struct Database *db = conn->db;
  for (size_t i = 0; i < conn->db->meta.max_data_rows; i++) {
    struct Address *cur = &db->rows[i];
    if (cur->meta.set) {
      Address_print(cur);
    }
  }
}

void Database_search(struct Connection *conn, const char *field,
                     const char *pattern) {
  if (conn == NULL || field == NULL || pattern == NULL) {
    die("Database_search: Invalid argument", conn);
  }

  size_t field_offset = 0;
  if (strcmp(field, "name") == 0) {
    field_offset = offsetof(struct Address, name);
  } else if (strcmp(field, "email") == 0) {
    field_offset = offsetof(struct Address, email);
  } else {
    die("Database_search: Field must be name or email", conn);
  }

  for (size_t i = 0; i < conn->db->meta.max_data_rows; i++) {
    struct Address *addr = &conn->db->rows[i];
    const char *value = *(char **)((char *)addr + field_offset);

    if (addr->meta.set && strstr(value, pattern) != NULL) {
      Address_print(addr);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3)
    die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

  char *filename = argv[1];
  char action = argv[2][0];

  struct Connection *conn = Database_open(filename, action);
  if (action == 'c') {
    if (argc != 4) {
      die("main: Need max_data_rows to create", conn);
    }
    int size_raw = atoi(argv[3]);
    if (size_raw <= 0) {
      die("main: Invalid max_data_rows", conn);
    }
    size_t size = (size_t)size_raw;
    struct DatabaseMeta meta = {size};
    Database_create(conn, meta);
    Database_write(conn);
    Database_close(conn);
    return 0;
  } else {
    Database_load(conn);
  }

  size_t id = 0;
  if (action != 'l' && action != 'f') {
    if (argc < 4)
      die("main: Need an id", conn);
    int id_raw = atoi(argv[3]);
    if (id_raw < 0 || (size_t)id_raw >= conn->db->meta.max_data_rows)
      die("main: There's no such record.", conn);
    id = (size_t)id_raw;
  }

  switch (action) {
  case 'g':
    if (argc != 4)
      die("main: Need an id to get", conn);
    Database_get(conn, id);
    break;
  case 's':
    if (argc != 6)
      die("main: Need id, name, email to set", conn);
    Database_set(conn, id, argv[4], argv[5]);
    Database_write(conn);
    break;
  case 'd':
    if (argc != 4)
      die("main: Need id to delete", conn);
    Database_delete(conn, id);
    Database_write(conn);
    break;
  case 'l':
    Database_list(conn);
    break;
  case 'f':
    if (argc != 5)
      die("main: Need field and pattern to search", conn);
    Database_search(conn, argv[3], argv[4]);
    break;
  default:
    die("main: Invalid action, only: c=create, g=get, d=del, l=list, f=find",
        conn);
  }
  Database_close(conn);

  return 0;
}
