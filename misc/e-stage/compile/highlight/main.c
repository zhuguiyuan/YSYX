#include "dbg.h"
#include "lexer.h"
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum {
  STYLE_NONE,
  STYLE_IDENT,
  STYLE_KEYWORD,
  STYLE_STRING,
  STYLE_COMMENT,
} style_t;

const char *keywords[] = {
    "auto",       "register", "static",   "extern",  "break", "case",
    "continue",   "default",  "do",       "else",    "for",   "goto",
    "if",         "return",   "switch",   "while",   "enum",  "struct",
    "union",      "char",     "double",   "float",   "int",   "long",
    "short",      "signed",   "unsigned", "void",    "const", "restrict",
    "volatile",   "inline",   "sizeof",   "typedef", "_Bool", "_Complex",
    "_Imaginary",
};

static inline bool is_digit(char c) { return '0' <= c && c <= '9'; }

static inline bool is_identifier_nodigit(char c) {
  return c == '_' || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static inline bool is_keyword(const char *str, size_t len) {
  for (size_t i = 0; i < G_N_ELEMENTS(keywords); ++i) {
    if (strlen(keywords[i]) == len && strncmp(keywords[i], str, len) == 0) {
      return true;
    }
  }
  return false;
}

/** 解析行注释 */
static inline style_t lex_line_comment(lexbuf_t *buf) {
  if (!lexbuf_eat_string(buf, "//")) {
    return STYLE_NONE;
  }
  while (lexbuf_peek(buf) != '\n' && lexbuf_peek(buf) != EOF) {
    lexbuf_next(buf);
  }
  return STYLE_COMMENT;
}

/** 解析块注释 */
static inline style_t lex_block_comment(lexbuf_t *buf) {
  if (!lexbuf_eat_string(buf, "/*")) {
    return STYLE_NONE;
  }
  char prev = 0;
  while (lexbuf_peek(buf) != EOF) {
    char c = lexbuf_next(buf);
    if (prev == '*' && c == '/') {
      break;
    }
    prev = c;
  }
  return STYLE_COMMENT;
}

static inline style_t lex_identifier(lexbuf_t *buf) {
  // [_a-zA-Z]([_a-zA-Z]|[0-9])*
  int start = buf->cursor.pos;
  if (!is_identifier_nodigit(lexbuf_peek(buf))) {
    return STYLE_NONE;
  }
  lexbuf_next(buf);
  while (is_identifier_nodigit(lexbuf_peek(buf)) ||
         is_digit(lexbuf_peek(buf))) {
    lexbuf_next(buf);
  }
  return is_keyword(buf->content + start, buf->cursor.pos - start)
             ? STYLE_KEYWORD
             : STYLE_IDENT;
}

/** 消费引号内的内容，处理 \\ 等转义;高亮器对坏代码宽容，所以不要求闭合 */
static inline void lex_quoted_body(lexbuf_t *buf, char quote) {
  while (true) {
    char c = lexbuf_peek(buf);
    if (c == quote || c == EOF) {
      break;
    }
    lexbuf_next(buf);
    if (c == '\\') {
      lexbuf_next(buf); /* 转义:连带跳过被转义的字符 */
    }
  }
}

static inline style_t lex_string(lexbuf_t *buf) {
  if (!lexbuf_eat(buf, '"')) {
    return STYLE_NONE;
  }
  lex_quoted_body(buf, '"');
  lexbuf_eat(buf, '"');
  return STYLE_STRING;
}

static inline style_t lex_char_literal(lexbuf_t *buf) {
  if (!lexbuf_eat(buf, '\'')) {
    return STYLE_NONE;
  }
  lex_quoted_body(buf, '\'');
  lexbuf_eat(buf, '\'');
  return STYLE_STRING;
}

typedef style_t (*lex_fn)(lexbuf_t *);
static const lex_fn rules[] = {
    lex_char_literal,  lex_string,       lex_identifier,
    lex_block_comment, lex_line_comment,
};

static const char *style_ctrl[] = {
    [STYLE_IDENT] = "",
    [STYLE_KEYWORD] = "\033[1;3;32m",
    [STYLE_STRING] = "\033[30m",
    [STYLE_COMMENT] = "\033[33m",
};

int main(int argc, char *argv[]) {
  g_autoptr(lexbuf_t) buf = NULL;
  check(argc >= 2, "usage: %s <file>", argv[0]);

  buf = read_file_to_lexbuf(argv[1]);
  check(buf != NULL, "failed to read file %s", argv[1]);
  cursor_t *cur = &buf->cursor;

  while (cur->pos < buf->length) {
    int start = cur->pos;
    style_t style = STYLE_NONE;
    for (size_t i = 0; i < G_N_ELEMENTS(rules) && style == STYLE_NONE; ++i) {
      style = rules[i](buf);
    }
    if (style != STYLE_NONE) {
      printf("%s%.*s\033[0m", style_ctrl[style], cur->pos - start,
             buf->content + start);
    } else {
      putchar(lexbuf_next(buf));
    }
  }
  puts("");

  /*
  // 这里还可以顺便测试一个注释 _Bool
  */

  return 0;
error:
  return EXIT_FAILURE;
}
