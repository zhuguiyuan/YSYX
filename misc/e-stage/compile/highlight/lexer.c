#include "lexer.h"
#include "dbg.h"
#include <stdbool.h>
#include <stdio.h>

void lexbuf_cleanup(lexbuf_t *buf) {
  if (buf != NULL) {
    g_free(buf->content);
  }
}

G_DEFINE_AUTOPTR_CLEANUP_FUNC(FILE, fclose)
lexbuf_t *read_file_to_lexbuf(const char *filename) {
  g_autoptr(FILE) file = NULL;
  g_autofree char *content = NULL;

  file = fopen(filename, "r");
  check(file != NULL, "failed to open file %s", filename);

  check(fseek(file, 0, SEEK_END) == 0, "failed to seek file end");
  long length = ftell(file);
  check(length >= 0, "ftell return negative value");
  check(fseek(file, 0, SEEK_SET) == 0, "failed to seek file begin");

  content = g_malloc0(length + 1);
  if (fread(content, sizeof(char), length, file) != length) {
    check(!ferror(file), "failed to read file content");
    sentinel("unexpected end of file");
  }

  lexbuf_t *buf = g_new0(lexbuf_t, 1);
  buf->filename = filename;
  buf->content = g_steal_pointer(&content);
  buf->length = length;
  return buf;

error:
  return NULL;
}

char lexbuf_peek(lexbuf_t *buf) {
  if (buf->cursor.pos >= buf->length)
    return EOF;
  return buf->content[buf->cursor.pos];
}

char lexbuf_next(lexbuf_t *buf) {
  if (buf->cursor.pos >= buf->length)
    return EOF;
  return buf->content[buf->cursor.pos++];
}

bool lexbuf_eat(lexbuf_t *buf, char c) {
  if (lexbuf_peek(buf) == c) {
    lexbuf_next(buf);
    return true;
  }
  return false;
}

bool lexbuf_eat_string(lexbuf_t *buf, const char *s) {
  int save = buf->cursor.pos;
  while (*s != '\0') {
    if (lexbuf_peek(buf) != *s) {
      buf->cursor.pos = save;
      return false;
    }
    lexbuf_next(buf);
    s += 1;
  }
  return true;
}
