#include <ctype.h>
#include <memory>
#include <stdio.h>

class Printer;

class PrinterState {
public:
  virtual ~PrinterState() {}
  virtual std::unique_ptr<PrinterState> HandleInput(Printer &printer, int c) {}
};

class InsideState : public PrinterState {
public:
  InsideState() = default;
};
class AfterState : public PrinterState {};
class BeforeState : public PrinterState {
public:
  virtual std::unique_ptr<PrinterState> HandleInput(Printer &printer, int c) {
    std::unique_ptr<PrinterState> next_state;
    InsideState test;
    if (c != ' ') {
      putchar(c);
      if (c != '\n') {
        *next_state = test;
        return next_state.reset(new InsideState());
      }
    }
    return next_state;
  }
};

class Printer {
public:
  virtual void HandleInput(int c) {
    auto next_state = state_->HandleInput(*this, c);
    if (next_state != nullptr)
      state_ = std::move(next_state);
  }

private:
  std::unique_ptr<PrinterState> state_;
};

int main() {
  Printer p;
  return 0;
}
