
#include "printer_states.h"
#include <ctype.h>
#include <iostream>
#include <memory>
#include <stdio.h>

void PrinterState::SetState(PrinterMachine &machine,
                            std::unique_ptr<PrinterState> state) {
  machine.state_ = std::move(state);
}

BeforeState::BeforeState() {
  std::cout << "Entering the Before State!" << std::endl;
}
BeforeState::~BeforeState() {
  std::cout << "Leaving the BeforeState!" << std::endl;
}

void BeforeState::ProcessChar(PrinterMachine &machine, int c) {
  if (c != ' ') {
    putchar(c);
    if (c != '\n') {
      std::unique_ptr<PrinterState> next_state(new InsideState);
      SetState(machine, std::move(next_state));
    }
  }
}

InsideState::InsideState() {
  std::cout << "Entering the InsideState!" << std::endl;
}
InsideState::~InsideState() {
  std::cout << "Leaving the InsideState! " << std::endl;
}
void InsideState::ProcessChar(PrinterMachine &machine, int c) {
  std::unique_ptr<PrinterState> next_state;
  if (!isspace(c))
    putchar(c);
  else {
    putchar('\n');
    if (c == '\n') {
      next_state.reset(new BeforeState);
      SetState(machine, std::move(next_state));
    } else {
      next_state.reset(new AfterState);
      SetState(machine, std::move(next_state));
    }
  }
}

AfterState::AfterState() {
  std::cout << "Entering the AfterState! " << std::endl;
}

AfterState::~AfterState() {
  std::cout << "Leaving the AfterState!" << std::endl;
}

void AfterState::ProcessChar(PrinterMachine &machine, int c) {
  std::unique_ptr<PrinterState> next_state;
  if (c == '\n') {
    next_state.reset(new BeforeState);
    SetState(machine, std::move(next_state));
  }
}
