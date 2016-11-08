#include <iostream>
#include <string>
// back-end
#include <boost/msm/back/state_machine.hpp>
// front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/euml/common.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front;

namespace {

// events
struct ProcessChar {
  int c;
};

// front-end: define the FSM structure
struct printer_machine_
    : public msm::front::state_machine_def<printer_machine_> {

  std::string output_string_;

  // The list of FSM states
  struct BeforeState : public msm::front::state<> {
    template <class Event, class FSM> void on_entry(Event const &, FSM &) {
      std::cout << "entering: BeforeState" << std::endl;
    }
    template <class Event, class FSM> void on_exit(Event const &, FSM &) {
      std::cout << "leaving: BeforeState" << std::endl;
    }
  };

  struct InsideState : public msm::front::state<> {
    template <class Event, class FSM> void on_entry(Event const &, FSM &) {
      std::cout << "entering: InsideState" << std::endl;
    }
    template <class Event, class FSM> void on_exit(Event const &, FSM &) {
      std::cout << "leaving: InsideState" << std::endl;
    }
  };

  struct AfterState : public msm::front::state<> {
    template <class Event, class FSM> void on_entry(Event const &, FSM &) {
      std::cout << "entering: AfterState" << std::endl;
    }
    template <class Event, class FSM> void on_exit(Event const &, FSM &) {
      std::cout << "leaving: AfterState" << std::endl;
    }
  };

  // the initial state of the player SM. Must be defined
  typedef BeforeState initial_state;

  // transition actions

  struct AppendChar {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &evt, FSM &fsm, SourceState &, TargetState &) {
      std::cout << "Appending Char" << std::endl;
      fsm.output_string_.push_back(static_cast<char>(evt.c));
    }
  };

  struct AppendNewLine {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &evt, FSM &fsm, SourceState &, TargetState &) {
      std::cout << "Appending Char" << std::endl;
      fsm.output_string_.push_back('\n');
    }
  };
  // guard conditions
  struct char_not_newline_or_space {
    template <class EVT, class FSM, class SourceState, class TargetState>
    bool operator()(EVT const &evt, FSM &fsm, SourceState &src,
                    TargetState &tgt) {
      if (evt.c != '\n' && evt.c != ' ')
        return true;
      return false;
    }
  };

  struct char_not_space {
    template <class EVT, class FSM, class SourceState, class TargetState>
    bool operator()(EVT const &evt, FSM &fsm, SourceState &src,
                    TargetState &tgt) {
      if (evt.c != ' ')
        return true;
      return false;
    }
  };

  struct char_newline {
    template <class EVT, class FSM, class SourceState, class TargetState>
    bool operator()(EVT const &evt, FSM &fsm, SourceState &src,
                    TargetState &tgt) {
      if (evt.c == '\n')
        return true;
      return false;
    }
  };

  typedef printer_machine_ p; // makes transition table cleaner

  // Transition table for player
  // clang-format off
  struct transition_table
      : mpl::vector<
            //    Start         Event             Next           Action           Guard
            //  +--------------+----------------+--------------+----------------+----------------------+
              Row<BeforeState  , ProcessChar    , BeforeState  , none           , none >,
              Row<BeforeState  , ProcessChar    , BeforeState  , AppendChar     , char_not_space >,
              Row<BeforeState  , ProcessChar    , InsideState  , AppendChar     , char_not_newline_or_space>,

              Row<InsideState  , ProcessChar    , AfterState   , AppendNewLine  ,  none>,
              Row<InsideState  , ProcessChar    , BeforeState  , AppendNewLine  , char_newline>,
              Row<InsideState  , ProcessChar    , InsideState  , AppendChar     , char_not_space>,

              Row<AfterState  , ProcessChar     , BeforeState  , AppendChar     , char_newline>
            //  +---------+-------------+---------+---------------------+----------------------+
            > {};
  // clang-format on
  // Replaces the default no-transition response.
  template <class FSM, class Event>
  void no_transition(Event const &e, FSM &, int state) {
    std::cout << "no transition from state " << state << " on event "
              << typeid(e).name() << std::endl;
  }
};
// Pick a back-end
typedef msm::back::state_machine<printer_machine_> my_machine;

//
// Testing utilities.
//
static char const *const state_names[] = {"State1", "State2", "State3",
                                          "State4"};
void pstate(my_machine const &p) {
  std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
}

void test() {
  my_machine p;
  p.start();
  p.process_event(ProcessChar{'h'});
  p.process_event(ProcessChar{'e'});
  p.process_event(ProcessChar{'l'});
  p.process_event(ProcessChar{'l'});
  p.process_event(ProcessChar{'o'});
}
}

int main() {
  test();
  return 0;
}
