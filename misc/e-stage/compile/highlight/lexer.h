#ifndef __lexer_h__
#define __lexer_h__

#include <glib.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct cursor {
  int pos;
} cursor_t;

typedef struct lexbuf {
  const char *filename;
  char *content;
  size_t length;
  cursor_t cursor;
} lexbuf_t;

lexbuf_t *read_file_to_lexbuf(const char *filename);
char lexbuf_peek(lexbuf_t *buf);
char lexbuf_next(lexbuf_t *buf);
bool lexbuf_eat(lexbuf_t *buf, char c);
bool lexbuf_eat_string(lexbuf_t *buf, const char *s);
void lexbuf_cleanup(lexbuf_t *buf);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(lexbuf_t, lexbuf_cleanup);

#endif