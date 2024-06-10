# 0 "src/parser/parser.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "src/parser/parser.c"
# 1 "./src/util/my_malloc.h" 1



# 1 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 1 3 4
# 145 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 3 4

# 145 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 214 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 329 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 3 4
typedef int wchar_t;
# 425 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 436 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 3 4
} max_align_t;
# 5 "./src/util/my_malloc.h" 2







# 11 "./src/util/my_malloc.h"
void *my_malloc(size_t size);
# 2 "src/parser/parser.c" 2
# 1 "./src/lexer/lexer.h" 1







# 1 "/usr/include/err.h" 1 3 4
# 22 "/usr/include/err.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 394 "/usr/include/features.h" 3 4
# 1 "/usr/include/features-time64.h" 1 3 4
# 20 "/usr/include/features-time64.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 21 "/usr/include/features-time64.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 1 3 4
# 19 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 20 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 2 3 4
# 22 "/usr/include/features-time64.h" 2 3 4
# 395 "/usr/include/features.h" 2 3 4
# 502 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
# 576 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 577 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 578 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 503 "/usr/include/features.h" 2 3 4
# 526 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
# 527 "/usr/include/features.h" 2 3 4
# 23 "/usr/include/err.h" 2 3 4


# 1 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stdarg.h" 3 4

# 40 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 26 "/usr/include/err.h" 2 3 4








extern void warn (const char *__format, ...)
     __attribute__ ((__format__ (__printf__, 1, 2))) __attribute__ ((__cold__));
extern void vwarn (const char *__format, __gnuc_va_list)
     __attribute__ ((__format__ (__printf__, 1, 0))) __attribute__ ((__cold__));


extern void warnx (const char *__format, ...)
     __attribute__ ((__format__ (__printf__, 1, 2)));
extern void vwarnx (const char *__format, __gnuc_va_list)
     __attribute__ ((__format__ (__printf__, 1, 0)));


extern void err (int __status, const char *__format, ...)
     __attribute__ ((__noreturn__, __format__ (__printf__, 2, 3))) __attribute__ ((__cold__));
extern void verr (int __status, const char *__format, __gnuc_va_list)
     __attribute__ ((__noreturn__, __format__ (__printf__, 2, 0))) __attribute__ ((__cold__));
extern void errx (int __status, const char *__format, ...)
     __attribute__ ((__noreturn__, __format__ (__printf__, 2, 3))) __attribute__ ((__cold__));
extern void verrx (int __status, const char *, __gnuc_va_list)
     __attribute__ ((__noreturn__, __format__ (__printf__, 2, 0))) __attribute__ ((__cold__));

# 1 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 1 3 4
# 120 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 25 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 2 3 4
# 121 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 2 3 4
# 56 "/usr/include/err.h" 2 3 4





# 9 "./src/lexer/lexer.h" 2
# 1 "./src/util/vector.h" 1





# 1 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 1 3 4
# 7 "./src/util/vector.h" 2






# 12 "./src/util/vector.h"
struct vector
{
  size_t size;
  size_t capacity;
  void** data;
};





void v_print(struct vector* v);






struct vector* v_create(void);






size_t v_size(struct vector* v);






int v_is_empty(struct vector* v);







size_t v_append(struct vector* v, void* elmt);







void* v_get(struct vector* v, size_t i);







void v_set(struct vector* v, size_t i, void* elmt);







void v_concat(struct vector* v1, struct vector* v2);






void* v_remove_last(struct vector* v);





void v_erase(struct vector *v);






void v_destroy(struct vector* v, void (*free_elt) (void*));
# 10 "./src/lexer/lexer.h" 2

enum terminal_symbol
{
  UNDIFINED = -1,
  SEMI,
  AND,
  PIPE,
  AND_IF,
  OR_IF,
  DSEMI,
  LESS,
  GREAT,
  DLESS,
  DGREAT,
  LESSAND,
  GREATAND,
  LESSGREAT,
  DLESSDASH,
  CLOBBER,
  L_BRACE,
  R_BRACE,
  L_PAR,
  R_PAR,
  BANG,
  EOF_SYM,
  IF,
  THEN,
  ELSE,
  ELIF,
  FI,
  DO,
  DONE,
  CASE,
  ESAC,
  WHILE,
  UNTIL,
  FOR,
  IN,
  FUNCTION,
  NL,
  WORD,
  ASSIGNMENT_WORD,
  NAME,
  IO_NUMBER
};

struct token
{
  enum terminal_symbol id;
  char* s;
};







int lexer(char* s, struct vector* v_token);





void token_destroy(void* p);
# 3 "src/parser/parser.c" 2
# 1 "src/parser/parser.h" 1



# 1 "./src/rules/rules.h" 1
# 12 "./src/rules/rules.h"
enum non_terminal_symbol
{
  SCRIPT,
  INPUT,
  LIST,
  COM_AMP,
  COM_AMP_NL,
  AND_OR,
  BOOL_OP,
  NEW_LINE_RULE,
  PIPELINE,
  BANG_RULE,
  COMMAND,
  SIMPLE_COMMAND,
  SHELL_COMMAND,
  FUNCDEC,
  REDIRECTION,
  IO_NUMBER_RULE,
  PREFIX,
  ELEMENT,
  COMPOUND_LIST,
  RULE_FOR,
  WORD_RULE,
  RULE_WHILE,
  RULE_UNTIL,
  RULE_CASE,
  RULE_IF,
  ELSE_CLAUSE,
  DO_GROUP,
  CASE_CLAUSE,
  DSEMI_RULE,
  CASE_ITEM,
  L_PAR_RULE,

  LIST_AUX,
  AND_OR_AUX,
  PIPELINE_AUX,
  COMPOUND_LIST_AUX,
  COMPOUND_LIST_AUX_2,
  FOR_RULE_AUX,
  FOR_RULE_AUX_2,
  CASE_CLAUSE_AUX,
  CASE_ITEM_AUX,
  NB_RULE
};

enum repeat
{
  MANDATORY,
  STAR,
  OPTIONAL,
  PLUS
};

struct symbol
{
  int terminal;
  enum repeat repeat;
  enum non_terminal_symbol rule;
  enum terminal_symbol terminal_symbol;
};

struct simple_rule
{
  struct symbol* sym_arr;
  size_t nb_sym;
};

struct rule
{
  struct simple_rule* s_r;
  size_t nb_s_r;
};

void create_sym(struct symbol* sym, int terminal, enum repeat repeat,
                enum non_terminal_symbol rule,
                enum terminal_symbol terminal_symbol);
void rules_destroy(struct rule** rules);





struct rule** init_all_rules(void);






struct rule** init_all_rules2(struct rule **rules);






struct rule** init_all_rules3(struct rule **rules);






struct rule** init_all_rules4(struct rule **rules);






struct rule** init_all_rules5(struct rule **rules);






struct rule** init_all_rules6(struct rule **rules);
# 5 "src/parser/parser.h" 2
# 15 "src/parser/parser.h"
struct tree* parse(struct rule** rules, struct vector* v_token, int* fit_level);
# 4 "src/parser/parser.c" 2

# 1 "./src/util/tree.h" 1




# 1 "./src/parser/parser.h" 1
# 6 "./src/util/tree.h" 2





struct tree
{
  struct token* token;
  enum non_terminal_symbol nts;
  struct vector* child;
};





void tree_print_dot(struct tree* tree);






struct tree* tree_create(enum non_terminal_symbol nts);






void tree_add_terminal_child(struct tree* tree, struct token* token);






void tree_add_non_terminal_child(struct tree* tree, struct tree* child);





void tree_delete_all_child(struct tree* tree);





void tree_destroy(struct tree* tree);







void tree_destroy_ast_extract_fun(struct tree* tree, struct vector* v_fun);





void tree_destroy_fun(void* p);
# 6 "src/parser/parser.c" 2


static struct tree* parse_rec(struct rule** rules, struct vector* v_token,
                              enum non_terminal_symbol symbol,
                              size_t* nb_token_read);

static struct tree* parse_rec_chap(struct rule** rules, struct vector* v_token,
                                   size_t* nb_token_read)
{
  *nb_token_read = 0;
  return parse_rec(rules, v_token, SCRIPT, nb_token_read);
}


struct tree* parse(struct rule** rules, struct vector* v_token, int* fit_level)
{
  size_t nb_token_read;
  *fit_level = 0;
  struct tree* tree = parse_rec_chap(rules, v_token, &nb_token_read);
  if (nb_token_read == v_size(v_token))
    return tree;
  else
  {
    tree_destroy(tree);
    if (nb_token_read == v_size(v_token))
      *fit_level = 1;
    return 
# 32 "src/parser/parser.c" 3 4
          ((void *)0)
# 32 "src/parser/parser.c"
              ;
  }
}

static int parse_mandatory(struct tree* tree, struct rule** rules,
                           struct vector* v_token, size_t* nb_token_read,
                           struct symbol* sym)
{
  struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
  if (child == 
# 41 "src/parser/parser.c" 3 4
              ((void *)0)
# 41 "src/parser/parser.c"
                  )
    return 0;
  tree_add_non_terminal_child(tree, child);
  return 1;
}

static int parse_star(struct tree* tree, struct rule** rules,
                      struct vector* v_token, size_t* nb_token_read,
                      struct symbol* sym)
{
  while (1)
  {
    struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
    if (child == 
# 54 "src/parser/parser.c" 3 4
                ((void *)0)
# 54 "src/parser/parser.c"
                    )
      break;
    tree_add_non_terminal_child(tree, child);
  }
  return 1;
}

static int parse_optional(struct tree* tree, struct rule** rules,
                          struct vector* v_token, size_t* nb_token_read,
                          struct symbol* sym)
{
  struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
  if (child != 
# 66 "src/parser/parser.c" 3 4
              ((void *)0)
# 66 "src/parser/parser.c"
                  )
    tree_add_non_terminal_child(tree, child);
  return 1;
}

static int parse_plus(struct tree* tree, struct rule** rules,
                      struct vector* v_token, size_t* nb_token_read,
                      struct symbol* sym)
{
  struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
  if (child == 
# 76 "src/parser/parser.c" 3 4
              ((void *)0)
# 76 "src/parser/parser.c"
                  )
    return 0;
  tree_add_non_terminal_child(tree, child);
  while (1)
  {
    struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
    if (child == 
# 82 "src/parser/parser.c" 3 4
                ((void *)0)
# 82 "src/parser/parser.c"
                    )
      break;
    tree_add_non_terminal_child(tree, child);
  }
  return 1;
}

static int parse_nonterminal(struct tree* tree, struct rule** rules,
                             struct vector* v_token, size_t* nb_token_read,
                             struct symbol* sym)
{
  if (sym->repeat == MANDATORY)
    return parse_mandatory(tree, rules, v_token, nb_token_read, sym);
  else if (sym->repeat == STAR)
    return parse_star(tree, rules, v_token, nb_token_read, sym);
  else if (sym->repeat == OPTIONAL)
    return parse_optional(tree, rules, v_token, nb_token_read, sym);
  else if (sym->repeat == PLUS)
    return parse_plus(tree, rules, v_token, nb_token_read, sym);
  else
    return 0;
}

static int parse_terminal(struct tree* tree, struct rule** rules,
                          struct vector* v_token, size_t* nb_token_read,
                          struct symbol* sym)
{
  rules = rules;
  struct token* token = v_get(v_token, *nb_token_read);
  if (token && sym->terminal_symbol == token->id)
  {
    *nb_token_read += 1;
    tree_add_terminal_child(tree, token);
    return 1;
  }
  return 0;
}



static struct tree* parse_rec(struct rule** rules, struct vector* v_token,
                              enum non_terminal_symbol symbol,
                              size_t* nb_token_read)
{
  struct tree* tree = tree_create(symbol);
  int nb_init_token_read = *nb_token_read;
  for (size_t i = 0; i < rules[symbol]->nb_s_r; ++i)
  {
    struct simple_rule* s_r = &(rules[symbol]->s_r[i]);
    tree_delete_all_child(tree);
    *nb_token_read = nb_init_token_read;
    size_t j;
    for (j = 0; j < s_r->nb_sym; ++j)
    {
      struct symbol* sym = &(s_r->sym_arr[j]);
      if (sym->terminal)
        if (!parse_terminal(tree, rules, v_token, nb_token_read, sym))
          break;
        else
          continue;

      else if (!parse_nonterminal(tree, rules, v_token, nb_token_read, sym))
        break;
    }
    if (j == s_r->nb_sym)
      return tree;
  }
  tree_destroy(tree);
  *nb_token_read = nb_init_token_read;
  return 
# 151 "src/parser/parser.c" 3 4
        ((void *)0)
# 151 "src/parser/parser.c"
            ;
}
