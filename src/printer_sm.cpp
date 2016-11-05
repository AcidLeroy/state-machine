#include "printer_sm.h"

PrinterMachine::PrinterMachine() : state_(new BeforeState) {}

void PrinterMachine::ProcessChar(int c) { state_->ProcessChar(*this, c); }
