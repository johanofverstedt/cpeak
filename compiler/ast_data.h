
#ifndef CPEAK_AST_DATA_H
#define CPEAK_AST_DATA_H

#include "ast.h"

struct ast_data_var {
  cstring name;
  usize   name_len;
  ast_ptr type;
};

inline
ast_ptr make_ast_var(arena a, cstring name, usize name_len, ast_ptr type) {
  ast_ptr result = make_ast_node(a, ast_tag_var, sizeof(ast_data_var));

  ast_data_var* data = AST_NODE_DATA(ast_data_var, result);

  data->name = name;
  data->name_len = name_len;
  data->type = type;

  return result;
}

inline
ast_ptr make_ast_var(arena a, cstring name, usize name_len, ast_ptr type, ast_ptr parent) {
  ast_ptr result = make_ast_var(a, name, name_len, type);

  add_last(parent, result);

  return result;
}

enum ast_type_category_enum {
  ast_type_category_integer,
  ast_type_category_float,
  ast_type_category_string,
  ast_type_category_char,
  ast_type_category_struct,
};

struct ast_data_type_atom {
  cstring name;
  usize   name_len;
  usize   category;
};

inline
ast_ptr make_ast_type_atom(arena a, cstring name, usize name_len, usize category) {
  ast_ptr result = make_ast_node(a, ast_tag_type_atom, sizeof(ast_data_type_atom));

  ast_data_type_atom* data = AST_NODE_DATA(ast_data_type_atom, result);

  data->name = name;
  data->name_len = name_len;
  data->category = category;  

  return result; 
}

inline
cstring ast_type_atom_category_name(usize category) {
  cstring result = "nil";

  switch(category) {
    case ast_type_category_integer:
      result = "<integer>";
      break;
    case ast_type_category_float:
      result = "<floating point>";
      break;
    case ast_type_category_string:
      result = "<string>";
      break;
    case ast_type_category_char:
      result = "<char>";
      break;
    case ast_type_category_struct:
      result = "<struct>";
      break;
  }
}

#endif
