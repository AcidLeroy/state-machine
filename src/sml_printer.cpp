// $CXX -std=c++14 -O2 -fno-exceptions -Wall -Wextra -Werror -pedantic
// -pedantic-errors tcp_release.cpp
// cl /std:c++14 /Ox /W3 tcp_release.cpp (***)

#include <boost/sml.hpp>
#include <cassert>

namespace sml = boost::sml;

// events
struct ProcessChar {
  int c;
};

// guards
const auto char_not_newline_or_space = [](const auto &event) {
  if (event.c != '\n' && event.c != ' ')
    return true;
  return false;
};

// actions
const auto AppendChar = [](int c) {};
const auto send_ack = [](const auto &event, sender &s) { s.send(event); };

struct PrinterMachine {
  auto operator()() const {
    using namespace sml;
    /**
     * Initial state: *initial_state
     * Transition DSL: src_state + event [ guard ] / action = dst_state
     */
    // clang-format off
    return make_transition_table(
        *"BeforeState"_s + event<ProcessChar>[char_not_newline_or_space] / AppendChar = "InsideState"_s,
        "fin wait 1"_s + event<ack>[is_valid] = "fin wait 2"_s,
        "fin wait 2"_s + event<fin>[is_valid] / send_ack = "timed wait"_s,
        "timed wait"_s + event<timeout> / send_ack = X
      );
    // clang-format on
  }
};

int main() {
  using namespace sml;

  sm<PrinterMachine> sm;
  assert(sm.is("BeforeState"_s));

  sm.process_event(ProcessChar{'h'});
}
