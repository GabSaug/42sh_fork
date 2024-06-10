# 0 "src/rules/rules4.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "src/rules/rules4.c"
# 1 "src/rules/rules.h" 1




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
# 6 "src/rules/rules.h" 2
# 1 "./src/util/my_malloc.h" 1



# 1 "/usr/lib/gcc/x86_64-linux-gnu/13/include/stddef.h" 1 3 4
# 5 "./src/util/my_malloc.h" 2






void *my_malloc(size_t size);
# 7 "src/rules/rules.h" 2





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
# 2 "src/rules/rules4.c" 2

static struct rule *init_rule_redirection2(struct rule *rule)
{

  rule->s_r[3].nb_sym = 3;
  rule->s_r[3].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[3].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[3].sym_arr + 1, 1, MANDATORY, 0, DLESS);
  create_sym(rule->s_r[3].sym_arr + 2, 1, MANDATORY, 0, WORD);


  rule->s_r[4].nb_sym = 3;
  rule->s_r[4].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[4].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[4].sym_arr + 1, 1, MANDATORY, 0, DLESSDASH);
  create_sym(rule->s_r[4].sym_arr + 2, 1, MANDATORY, 0, WORD);


  rule->s_r[5].nb_sym = 3;
  rule->s_r[5].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[5].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[5].sym_arr + 1, 1, MANDATORY, 0, GREATAND);
  create_sym(rule->s_r[5].sym_arr + 2, 1, MANDATORY, 0, WORD);

  return rule;
}

static struct rule *init_rule_redirection3(struct rule *rule)
{

  rule->s_r[6].nb_sym = 3;
  rule->s_r[6].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[6].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[6].sym_arr + 1, 1, MANDATORY, 0, LESSAND);
  create_sym(rule->s_r[6].sym_arr + 2, 1, MANDATORY, 0, WORD);


  rule->s_r[7].nb_sym = 3;
  rule->s_r[7].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[7].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[7].sym_arr + 1, 1, MANDATORY, 0, CLOBBER);
  create_sym(rule->s_r[7].sym_arr + 2, 1, MANDATORY, 0, WORD);


  rule->s_r[8].nb_sym = 3;
  rule->s_r[8].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[8].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[8].sym_arr + 1, 1, MANDATORY, 0, LESSGREAT);
  create_sym(rule->s_r[8].sym_arr + 2, 1, MANDATORY, 0, WORD);

  return rule;
}

static struct rule *init_rule_redirection(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 9;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);

  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, GREAT);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, WORD);


  rule->s_r[1].nb_sym = 3;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[1].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[1].sym_arr + 1, 1, MANDATORY, 0, LESS);
  create_sym(rule->s_r[1].sym_arr + 2, 1, MANDATORY, 0, WORD);


  rule->s_r[2].nb_sym = 3;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[2].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[2].sym_arr + 1, 1, MANDATORY, 0, DGREAT);
  create_sym(rule->s_r[2].sym_arr + 2, 1, MANDATORY, 0, WORD);

  rule = init_rule_redirection2(rule);
  rule = init_rule_redirection3(rule);
  return rule;
}

static struct rule *init_rule_io_number(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));

  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, IO_NUMBER);

  return rule;
}

static struct rule *init_rule_prefix(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);

  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, ASSIGNMENT_WORD);


  rule->s_r[1].nb_sym = 1;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 0, MANDATORY, REDIRECTION, 0);

  return rule;
}

static struct rule *init_rule_element(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);

  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, WORD);


  rule->s_r[1].nb_sym = 1;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 0, MANDATORY, REDIRECTION, 0);

  return rule;
}

static struct rule *init_rule_compound_list(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);

  rule->s_r[0].nb_sym = 4;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 4);
  create_sym(rule->s_r[0].sym_arr + 0, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, AND_OR, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, COMPOUND_LIST_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 0, OPTIONAL, COMPOUND_LIST_AUX_2, 0);

  return rule;
}

static struct rule *init_rule_compound_list_aux(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);

  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, COM_AMP_NL, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, MANDATORY, AND_OR, 0);

  return rule;
}

static struct rule *init_rule_compound_list_aux_2(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);

  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, COM_AMP_NL, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);

  return rule;
}

struct rule **init_all_rules4(struct rule **rules)
{
  rules[REDIRECTION] = init_rule_redirection();
  rules[IO_NUMBER_RULE] = init_rule_io_number();
  rules[PREFIX] = init_rule_prefix();
  rules[ELEMENT] = init_rule_element();
  rules[COMPOUND_LIST] = init_rule_compound_list();
  rules[COMPOUND_LIST_AUX] = init_rule_compound_list_aux();
  rules[COMPOUND_LIST_AUX_2] = init_rule_compound_list_aux_2();

  return rules;
}
