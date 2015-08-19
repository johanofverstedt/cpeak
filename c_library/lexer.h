
#ifndef CPEAK_LEXER_H
#define CPEAK_LEXER_H

#include "types.h"
#include "macro.h"
#include "arena.h"

#include <stdio.h>

enum token_tag_category {
  token_tag_category_invalid,
  token_tag_category_identifier,
  token_tag_category_numeric_literal,
  token_tag_category_string_literal,
  token_tag_category_char_literal,

  token_tag_category_operator,
};

enum token_tag_enum {
  token_tag_invalid = 0,

  // keywords
  token_tag_def = 1,
  token_tag_struct = 2,
  token_tag_if = 3,
  token_tag_while = 4,

  // classification of non-keyword identifiers and literals
  token_tag_identifier = 5,
  token_tag_integer_literal = 6,
  token_tag_hex_literal = 7,
  token_tag_float_literal = 8,
  token_tag_string_literal = 9,
  token_tag_char_literal = 10,

  // operators
  token_tag_op_left_param = 11,
  token_tag_op_right_param = 12,
  token_tag_op_left_brace = 13,
  token_tag_op_right_brace = 14,
  token_tag_op_left_bracket = 15,
  token_tag_op_right_bracket = 16,

  token_tag_op_add = 17,    // +
  token_tag_op_sub = 18,    // -
  token_tag_op_negate = 18, // -
  token_tag_op_mul = 19,    // *
  token_tag_op_div = 20,    // /
  token_tag_op_mod = 21,    // %

  token_tag_op_and,         // &&
  token_tag_op_or,          // ||
  token_tag_op_not,         // !
  token_tag_op_question,    // ?
  token_tag_op_colon,       // :
  token_tag_op_semicolon,   // ;
  token_tag_op_dot,         // .
  token_tag_op_comma,       // ,
  token_tag_op_dollar,      // $
  token_tag_op_hash,        // #

  token_tag_op_equality,              // ==
  token_tag_op_inequality,            // !=
  token_tag_op_less_than,             // <
  token_tag_op_greater_than,          // >
  token_tag_op_less_than_or_equal,    // <=
  token_tag_op_greater_than_or_equal, // >=

  token_tag_op_assignment,      // =
  token_tag_op_decl_assignment, // :=
  token_tag_op_left_arrow,      // <-
  token_tag_op_right_arrow,     // ->

  token_tag_op_left_shift,      // <<
  token_tag_op_right_shift,     // >>

  token_tag_op_ref,             // &
  token_tag_op_deref,           // *
};

struct lexer_state {
  cstring str;
  u32     str_len;
  u32     pos;
  u32     row;
  u32     col;

  arena   ma;
};

struct token {
  u32 tag;
  u32 start;
  u32 length;
  u32 row;
  u32 col;
};

typedef token* token_ptr;

void print_token(token_ptr tok, lexer_state* lex) {
  cstring str = lex->str;
  arena a     = lex->ma;
  
  if(tok == 0) {
    const char str[] = "[null token]";
    fwrite(str, 1, sizeof(str), stdout);
  } else {
    u32 tok_length = tok->length;

    const char preamble[] = "[";
    const char postamble[] = "] @";
    char tag_buf[32];
    sprintf(tag_buf, "tag: %d, ", tok->tag);

    fwrite(preamble, 1, sizeof(preamble) - 1, stdout);
    fwrite(tag_buf, 1, strlen(tag_buf), stdout);
    sprintf(tag_buf, "len: %u, ", tok_length);
    fwrite(tag_buf, 1, strlen(tag_buf), stdout);

    fwrite(str + tok->start, 1, tok_length, stdout);

    fwrite(postamble, 1, sizeof(postamble) - 1, stdout);

    sprintf(tag_buf, "L%uC%u", tok->row, tok->col);
    fwrite(tag_buf, 1, strlen(tag_buf), stdout);
  }
}

i32 compare_cstrings(cstring a, cstring b, u32 count) {
  for(u32 i = 0; i < count; ++i) {
    if(a[i] != b[i]) {
      return (i32)a[i] - (i32)b[i];
    }
  }

  return 0;
}

lexer_state* make_lexer(arena ma, cstring str, u32 str_len) {
  lexer_state* result;

  result = (lexer_state*)arena_alloc(ma, sizeof(lexer_state));

  result->str = str;
  result->str_len = str_len;
  result->pos = 0;
  result->row = 1;
  result->col = 1;
  result->ma  = ma;

  return result;
}

token_ptr lexer_enclosed_literal(lexer_state* lex, token_tag_enum tag, char terminator) {
  token_ptr result = 0;

  cstring str = lex->str;
  u32 str_len = lex->str_len;
  u32 pos = lex->pos;
  u32 row = lex->row;
  u32 col = lex->col;

  ++pos;

  result = (token_ptr)arena_alloc(lex->ma, sizeof(token));  
  result->tag = tag;
  result->start = pos;

  ++col;
  
  result->row = row;
  result->col = col;

  char c = str[pos];
  bool backslash = false;
  while(pos < str_len) {
    if(backslash) {
      ++col;
      backslash = false;
    } else {
      if(c == terminator) {
        ++pos;
        ++col;
        break;
      }
      if(c == '\\') {
        backslash = true;
      }
    }
    if(c == '\n') {
      ++row;
      col = 1;
    }
    c = str[++pos];
  }

  result->length = (pos - result->start) - 1U;

  lex->pos = pos;
  lex->row = row;
  lex->col = col;

  return result;
}

token_ptr lexer_hex_literal(lexer_state* lex) {
  token_ptr result = 0;

  cstring str = lex->str;
  u32 str_len = lex->str_len;
  u32 pos = lex->pos;
  u32 row = lex->row;
  u32 col = lex->col;

  u32 start = pos;

  printf("lex hex\n");

  result = (token_ptr)arena_alloc(lex->ma, sizeof(token));
  result->tag = token_tag_hex_literal;
  result->start = pos;
  result->row = row;
  result->col = col;

  bool not_done = true;

  while(pos < str_len && not_done) {
    char c = str[pos];
    not_done = ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));

    if(not_done) {
      ++pos;
      ++col;
    }
  }
  
  result->length = pos - start;

  lex->pos = pos;
  lex->col = col;

  return result;
}

token_ptr lexer_numeric_literal(lexer_state* lex) {
  token_ptr result = 0;

  cstring str = lex->str;
  u32 str_len = lex->str_len;
  u32 pos = lex->pos;
  u32 row = lex->row;
  u32 col = lex->col;

  if(pos >= str_len)
    return result;

  // detect special case: hex form 0x....

  if(pos + 2 < str_len && str[pos] == '0' && str[pos + 1] == 'x') {
    pos += 2;
    col += 2;

    lex->pos = pos;
    lex->col = col;

    result = lexer_hex_literal(lex);
  } else {
    u32 start = pos;

    result = (token_ptr)arena_alloc(lex->ma, sizeof(token));
    result->start = pos;
    result->row = row;
    result->col = col;

    bool not_done = true;
    bool is_float = false;

    while(pos < str_len && not_done) {
      char c = str[pos];

      if(c >= '0' && c <= '9') {
        ++pos;
        ++col;
      } else if(c == '.') {
        ++pos;
        ++col;

        not_done = false;
        is_float = true;
      } else {
        not_done = false;
      }
    }

    if(is_float) {
      not_done = true;

      while(pos < str_len && not_done) {
        char c = str[pos];

        if(c < '0' || c > '9') {
          not_done = false;
        } else {
          ++pos;
          ++col;
        }
      }

      result->length = pos - start;
      result->tag = token_tag_float_literal;

      lex->pos = pos;
      lex->col = col;
    } else {
      result->length = pos - start;
      result->tag = token_tag_integer_literal;

      lex->pos = pos;
      lex->col = col;
    }
  }

  return result;
}

inline
token_ptr lexer_operator_1(lexer_state* lex) {
  token_ptr result;

  cstring str = lex->str;
  u32 str_len = lex->str_len;
  u32 pos = lex->pos;
  u32 row = lex->row;
  u32 col = lex->col;

  result = (token_ptr)arena_alloc(lex->ma, sizeof(token));
  
  result->start = pos;
  result->length = 1U;
  result->row = row;
  result->col = col;

  switch(str[pos]) {
    case '(':
      result->tag = token_tag_op_left_param;
      break;
    case ')':
      result->tag = token_tag_op_right_param;
      break;
    case '[':
      result->tag = token_tag_op_left_bracket;
      break;
    case ']':
      result->tag = token_tag_op_right_bracket;
      break;
    case '{':
      result->tag = token_tag_op_left_brace;
      break;
    case '}':
      result->tag = token_tag_op_right_brace;
      break;
    case '+':
      result->tag = token_tag_op_add;
      break;
    case '-':
      result->tag = token_tag_op_sub;
      break;
    case '*':
      result->tag = token_tag_op_mul;
      break;
    case '/':
      result->tag = token_tag_op_div;
      break;
  }

  lex->pos = pos + 1U;
  lex->col = col + 1U;

  return result;
}

inline
token_ptr lexer_process(lexer_state* lex) {
  token_ptr result = 0;

  cstring str = lex->str;
  u32 str_len = lex->str_len;
  u32 pos = lex->pos;
  
  if(pos >= str_len)
    return result; // reached end

  // skip whitespace

  bool not_done_skipping_ws = true;

  u32 row = lex->row;
  u32 col = lex->col;

  while(not_done_skipping_ws && pos < str_len) {
    char c = str[pos];

    switch(c) {
      case ' ':
        ++pos;
        ++col;
        break;
      case '\t':
        ++pos;
        col += 2;
        break;
      case '\n':
        ++pos;
        ++row;
        col = 1;
        break;
      default:
        not_done_skipping_ws = false;
        break;
    }
  }

  lex->row = row;
  lex->col = col;
  lex->pos = pos;

  switch(str[pos]) {
    case '\"': // string literal
      result = lexer_enclosed_literal(lex, token_tag_string_literal, '\"');
      break;
    case '\'': // char literal
      result = lexer_enclosed_literal(lex, token_tag_char_literal, '\'');
      break;
    case '0': // numeric literal
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      result = lexer_numeric_literal(lex);
      break;
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
      result = lexer_operator_1(lex);
      break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '&':
    case '|':
      result = lexer_operator_1(lex);
      break;
  }

  return result;
}

#endif
