
#include "ast.h"
#include "ast_data.h"
#include "ast_printer.h"

int main(int argc, char** argv) {
  usize id = 0;
  arena a = make_system_arena(1024U*1024U);
  ast_ptr file_scope = make_ast_node(a, ast_tag_scope, 0ULL);

  ast_ptr func_foo = make_ast_node(a, ast_tag_def, 0ULL, file_scope);
  ast_ptr param0 = make_ast_var(a, "x_var", 5, make_ast_type_atom(a, "u32", 3, ast_type_category_integer));
  add_last(func_foo, param0);
  //make_ast_node(a, ast_tag_var, sizeof(ast_tag_var), func_foo);
  //ast_ptr param1 = make_ast_node(a, ast_tag_var, sizeof(ast_tag_var), func_foo);
  //ast_ptr ret0 = make_ast_node(a, ast_tag_var, sizeof(ast_tag_var), func_foo);

    

  print(file_scope);

  return 0;
}