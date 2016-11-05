#ifndef _SRC_PRINTER_STATES_H_
#define _SRC_PRINTER_STATES_H_

#include "printer_sm.h"
#include <memory>

class PrinterMachine;

class PrinterState {
public:
  virtual void ProcessChar(PrinterMachine &machine, int c){};
  virtual ~PrinterState(){};

protected:
  void SetState(PrinterMachine &machine, std::unique_ptr<PrinterState> state);
};

class BeforeState : public PrinterState {
public:
  BeforeState();
  ~BeforeState();
  virtual void ProcessChar(PrinterMachine &machine, int c) override;
};

class InsideState : public PrinterState {
public:
  InsideState();
  ~InsideState();
  virtual void ProcessChar(PrinterMachine &machine, int c) override;
};

class AfterState : public PrinterState {
public:
  AfterState();
  ~AfterState();
  virtual void ProcessChar(PrinterMachine &machine, int c) override;
};

#endif
