
#ifndef CPEAK_AST_STATEMENT_H
#define CPEAK_AST_STATEMENT_H

#include "../c_library/arena.h"
#include "../c_library/types.h"

enum ast_tag_enum {
  ast_tag_nil,

  //statements
  ast_tag_decl,
  ast_tag_decl_assign,
  ast_tag_assign,
  ast_tag_def,
  ast_tag_def_expr,
  ast_tag_struct,
  ast_tag_scope,
  ast_tag_import,

  ast_tag_type_atom,
  ast_tag_type_call,

  ast_tag_value,
  ast_tag_var,
  ast_tag_tuple,
  ast_tag_call,
};

struct ast {
  ast* parent;
  ast* child;
  ast* next;
  usize tag;
};

typedef ast* ast_ptr;

ast* ast_nil() {
  ast* result;
  static ast node_storage = {0};

  result = &node_storage;

  return result;
}

ast* get_last(ast* node) {
  while(node->next) {
    node = node->next;
  }

  return node;
}

ast* add_first(ast* parent, ast* child) {
  child->parent = parent;
  child->next = parent->child;
  parent->child = child;

  return child;
}

ast* add_after(ast* predecessor, ast* new_successor) {
  new_successor->parent = predecessor->parent;
  new_successor->next = predecessor->next;
  predecessor->next = new_successor;

  return new_successor;
}

ast* add_last(ast* parent, ast* child) {
  ast* result = child;
  ast* first = parent->child;

  if(first == 0) {
    parent->child = child;

    child->parent = parent;
  } else {
    add_after(get_last(first), child);
  }

  return result;
}

void reverse_children(ast* parent) {
  ast* first = parent->child;
  ast* new_list = 0;

  // empty list => no-op
  if(first == 0)
    return;

  new_list = first;
  first = first->next;

  while(first) {
    ast* old_first = first;
    first = first->next;
    old_first->next = new_list;
    new_list = old_first;
  }

  parent->child = new_list;
}

#define AST_NODE_PADDED_SIZE sizeof(ast)
#define AST_NODE_DATA(type, node_ptr) (type*)(((u8*)(node_ptr)) + AST_NODE_PADDED_SIZE)

ast* make_ast_node(arena a, usize tag, usize data_size) {
  ast* result;

  result = (ast*)arena_alloc(a, AST_NODE_PADDED_SIZE + data_size);

  result->parent = 0;
  result->child = 0;
  result->next = 0;
  result->tag = tag;

  return result;
}

ast* make_ast_node(arena a, usize tag, usize data_size, ast* parent) {
  ast* result = make_ast_node(a, tag, data_size);

  add_last(parent, result);

  return result;
}

#endif
