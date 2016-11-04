#include <ctype.h>
#include <stdio.h>

int main(void) {
  int c;
  do {
    do
      c = getchar();
    while (c == ' ');
    while (c != EOF && !isspace(c) && c != '\n') {
      putchar(c);
      c = getchar();
    }
    putchar('\n');
    while (c != EOF && c != '\n')
      c = getchar();
  } while (c != EOF);
  return 0;
}
