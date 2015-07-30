
#include "lexer.h"

token_ptr process_next(lexer_state* lex) {
  token_ptr tok = lexer_process(lex);

  const char newline_str[] = "\n";
  print_token(tok, lex);
  fwrite(newline_str, 1, 1, stdout);

  fflush(stdout);

  printf("row: %u, col: %u.\n", lex->row, lex->col);

  fflush(stdout);

  return tok;
}

int main(int argc, void** argv) {
  const char str[] =
    "0xdeadbeef "
    "0xDEADBEEF \n"
    "00005000 "
    "05.5 \n " 
    "0 \n"
    " \"galttjosan\" \'a\'";

  arena a = make_system_arena(1024U * 1024U);

  lexer_state* lex = make_lexer(a, str, sizeof(str));

  token_ptr tok;

  do {
    tok = process_next(lex);
  } while(tok);

  free_system_arena(a);

  return 0;
}
