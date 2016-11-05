#ifndef _SRC_PRINTER_SM_H_
#define _SRC_PRINTER_SM_H_
#include "printer_states.h"

#include <memory>

class PrinterState;

class PrinterMachine {
  friend class PrinterState;

public:
  PrinterMachine();
  void ProcessChar(int c);
  ~PrinterMachine(){};

private:
  std::unique_ptr<PrinterState> state_;
};

#endif
