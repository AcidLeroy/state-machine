#include <ctype.h>
#include <stdio.h>

int main(void) {
  enum states { before, inside, after } state;
  int c;
  state = before;
  while ((c = getchar()) != EOF) {
    switch (state) {
    case before:
      if (c != ' ') {
        putchar(c);
        if (c != '\n')
          state = inside;
      }
      break;
    case inside:
      if (!isspace(c))
        putchar(c);
      else {
        putchar('\n');
        if (c == '\n')
          state = before;
        else
          state = after;
      }
      break;
    case after:
      if (c == '\n')
        state = before;
    }
  }
  return 0;
}
