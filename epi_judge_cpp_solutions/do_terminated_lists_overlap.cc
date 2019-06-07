#include <memory>

#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using std::shared_ptr;


shared_ptr<ListNode<int>> OverlappingNoCycleLists(
    shared_ptr<ListNode<int>> l0, shared_ptr<ListNode<int>> l1) {
    auto head1 = l0;
    auto head2 = l1;
    while(head1 != head2)
    {
        if(head1 == nullptr)
            head1 = l1;
        if(head2 == nullptr)
            head2 = l0;
        head1 = head1->next;
        head2 = head2->next;
    }
  return head1;
}

void OverlappingNoCycleListsWrapper(TimedExecutor& executor,
                                    shared_ptr<ListNode<int>> l0,
                                    shared_ptr<ListNode<int>> l1,
                                    shared_ptr<ListNode<int>> common) {
  if (common) {
    if (l0) {
      auto i = l0;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l0 = common;
    }

    if (l1) {
      auto i = l1;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l1 = common;
    }
  }

  auto result = executor.Run([&] { return OverlappingNoCycleLists(l0, l1); });

  if (result != common) {
    throw TestFailure("Invalid result");
  }
}

// clang-format off


int main(int argc, char* argv[]) {
  std::vector<std::string> args {argv + 1, argv + argc};
  std::vector<std::string> param_names {"executor", "l0", "l1", "common"};
  return GenericTestMain(args, "do_terminated_lists_overlap.cc", "do_terminated_lists_overlap.tsv", &OverlappingNoCycleListsWrapper, DefaultComparator{}, param_names);
}
// clang-format on
