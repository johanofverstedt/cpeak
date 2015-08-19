
#ifndef CPEAK_AST_PRINTER_H
#define CPEAK_AST_PRINTER_H

#include "ast.h"
#include "ast_data.h"

inline
void print_cstring(cstring s, usize len) {
  for(usize i = 0; i < len; ++i) {
    printf("%c", s[i]);
  }
}

void print_one(ast* tree, usize level) {
  u32 parent_tag = 0;
  u32 current_tag = (u32)tree->tag;
  if(tree->parent) { parent_tag = (u32)tree->parent->tag; }

  switch(current_tag) {
    case ast_tag_nil:
      printf("NIL\n");
      break;
    case ast_tag_var:
      {
        ast_data_var* data = AST_NODE_DATA(ast_data_var, tree);
        print_cstring(data->name, data->name_len);
        printf(": ");
        print_one(data->type, level + 1);
        break;
      }
    case ast_tag_type_atom:
      {
        ast_data_type_atom* data = AST_NODE_DATA(ast_data_type_atom, tree);
        print_cstring(data->name, data->name_len);
        break;
      }
    default: {
      printf("<%u (%u)>", (u32)tree->tag, parent_tag);
      break;
    }
  }
}
void print_rec(ast* tree, usize level) {
  if(tree == 0)
    return;

  print_one(tree, level);

  usize next_level = level + 1;

  ast* it = tree->child;
  while(it) {
    print_rec(it, next_level);
    it = it->next;
  }
}
void print(ast* tree) {
  if(tree == 0)
    return;

  print_rec(tree, 0);
}

#endif
