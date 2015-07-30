
#ifndef CPEAK_SYNTAX_STRUCTS_H
#define CPEAK_SYNTAX_STRUCTS_H

#include "types.h"
#include "alloc.h"
#include <string.h>

enum statement_id_enum {
  statement_id_def,
  statement_id_struct,
  statement_id_decl_var,
  statement_id_assign,
  statement_id_if,
  statement_id_while,
  statement_id_count,
};

enum expr_id_enum {
  expr_id_literal,
  expr_id_var,
  
  // arithmetic

  expr_id_add,
  expr_id_sub,
  expr_id_mul,
  expr_id_div,

  expr_id_ref,
  expr_id_deref,
};

struct statement {
  u32 id;
  expr* lhs;
  expr* rhs;
  statement* scope;
};

struct expr {
  u32 id;
  u32 arity;
  expr* children[3];
  cstring identifier;
};

inline
statement* make_statement(allocator a, u32 id) {
  statement* result = (statement*)cpeak_alloc(sizeof(statement));

  result->id = id;
  result->lhs = 0;
  result->rhs = 0;
  result->scope = 0;

  return result;
}

inline
statement* make_statement(allocator a, u32 id, expr* lhs, expr* rhs) {
  statement* result = (statement*)cpeak_alloc(sizeof(statement));

  result->id = id;
  result->lhs = lhs;
  result->rhs = rhs;
  result->scope = 0;

  return result;
}

inline
statement* make_statement(allocator a, u32 id, statement* scope) {
  statement* result = (statement*)cpeak_alloc(sizeof(statement));

  result->id = id;
  result->lhs = 0;
  result->rhs = 0;
  result->scope = scope;

  return result;
}

inline
expr* make_expr(allocator a, u32 id) {
  expr* result = (expr*)cpeak_alloc(sizeof(expr));

  result->id = id;
  result->arity = 0U;
  result->children[0] = 0;
  result->children[1] = 0;
  result->children[2] = 0;

  return result;
}

inline
expr* make_expr(allocator a, u32 id, expr* child) {
  expr* result = (expr*)cpeak_alloc(sizeof(expr));

  result->id = id;
  result->arity = 1U;
  result->children[0] = child;
  result->children[1] = 0;
  result->children[2] = 0;

  return result;
}

inline
expr* make_expr(allocator a, u32 id, expr* child1, expr* child2) {
  expr* result = (expr*)cpeak_alloc(sizeof(expr));

  result->id = id;
  result->arity = 2U;
  result->children[0] = child1;
  result->children[1] = child2;
  result->children[2] = 0;

  return result;
}

inline
expr* make_expr(allocator a, u32 id, expr* child1, expr* child2, expr* child3) {
  expr* result = (expr*)cpeak_alloc(sizeof(expr));

  result->id = id;
  result->arity = 3U;
  result->children[0] = child1;
  result->children[1] = child2;
  result->children[2] = child3;

  return result;
}

#endif
