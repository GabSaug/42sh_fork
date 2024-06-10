# 0 "src/util/my_math.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "src/util/my_math.c"
# 1 "src/util/my_math.h" 1



long int my_pow(long int n, long int p);
# 2 "src/util/my_math.c" 2

long int my_pow(long int n, long int p)
{
  int res = 1;
  for (; p > 0; p--)
    res *= n;
  return res;
}
