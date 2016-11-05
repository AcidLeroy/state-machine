#include "printer_sm.h"
#include "printer_states.h"
#include <ctype.h>
#include <stdio.h>

int main() {
  PrinterMachine p;
  int c;
  while ((c = getchar()) != EOF) {
    p.ProcessChar(c);
  }
  return 0;
}
