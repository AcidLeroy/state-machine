// $CXX -std=c++14 -O2 -fno-exceptions -Wall -Wextra -Werror -pedantic
// -pedantic-errors tcp_release.cpp
// cl /std:c++14 /Ox /W3 tcp_release.cpp (***)

#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <string>

namespace sml = boost::sml;

// events -- Events can contain any type of data
struct ProcessChar {
  int c;
};

// Dependencies this is where we can store "extended state". In this case I
// stored them in a single string.
struct Dependencies {
  std::string word;
};

// guards
const auto is_space = [](const auto &event) { return event.c == ' '; };
const auto is_newline = [](const auto &event) { return event.c == '\n'; };

// actions
auto AppendChar = [](Dependencies &d, const auto &event) {
  std::cout << "Appending character \"" << static_cast<char>(event.c) << "\""
            << std::endl;
  d.word.push_back(static_cast<char>(event.c));
  std::cout << "The current output is now : " << d.word << std::endl;

};

auto AppendNewLine = [](Dependencies &d, const auto &event) {
  std::cout << "Appending NewLine!" << std::endl;
  std::cout << "Character received is: " << event.c << std::endl;
  d.word.push_back('\n');
  std::cout << "The current output is: " << d.word << std::endl;

};

struct PrinterMachine {
  auto operator()() const {
    using namespace sml;

    /**
     * Initial state: *initial_state
     * Transition DSL: src_state + event [ guard ] / action = dst_state
     */
    // clang-format off
    return make_transition_table(
     *"BeforeState"_s + event<ProcessChar>[!is_space && !is_newline] / AppendChar = "InsideState"_s,
     "BeforeState"_s + event<ProcessChar>[!is_space] / AppendChar = "BeforeState"_s,

     "InsideState"_s + event<ProcessChar>[!is_space] / AppendChar = "InsideState"_s,
     "InsideState"_s + event<ProcessChar>[is_newline] / AppendChar = "BeforeState"_s,
     "InsideState"_s + event<ProcessChar> / AppendNewLine = "AfterState"_s,

     "AfterState"_s + event<ProcessChar>[is_newline] / AppendNewLine = "BeforeState"_s
      );
    // clang-format on
  }
};

int main() {
  using namespace sml;

  Dependencies d;
  sm<PrinterMachine> sm{d};
  std::cout << "Created the machine..." << std::endl;
  // assert(sm.is("BeforeState"_s));
  sm.process_event(ProcessChar{' '});
  assert(sm.is("BeforeState"_s));

  sm.process_event(ProcessChar{'\n'});
  assert(sm.is("BeforeState"_s));

  sm.process_event(ProcessChar{'h'});
  assert(sm.is("InsideState"_s));
  sm.process_event(ProcessChar{'e'});
  assert(sm.is("InsideState"_s));
  sm.process_event(ProcessChar{'l'});
  assert(sm.is("InsideState"_s));
  sm.process_event(ProcessChar{'l'});
  assert(sm.is("InsideState"_s));
  sm.process_event(ProcessChar{'o'});
  assert(sm.is("InsideState"_s));

  sm.process_event(ProcessChar{' '});
  assert(sm.is("AfterState"_s));
  sm.process_event(ProcessChar{'\n'}); // Simulate new line
  assert(sm.is("BeforeState"_s));

  sm.process_event(ProcessChar{'w'});
  assert(sm.is("InsideState"_s));
  sm.process_event(ProcessChar{'o'});
  assert(sm.is("InsideState"_s));
  sm.process_event(ProcessChar{'r'});
  assert(sm.is("InsideState"_s));
  sm.process_event(ProcessChar{'l'});
  assert(sm.is("InsideState"_s));
  sm.process_event(ProcessChar{'d'});
  assert(sm.is("InsideState"_s));
}
