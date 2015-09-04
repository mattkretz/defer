#include "unittest.h"
#include "../defer.h"

struct A {
  A() { ++n_objects; }
  ~A() { --n_objects; }
  static int n_objects;
};

int A::n_objects = 0;

TEST(defer_delete) {
  {
    COMPARE(A::n_objects, 0);
    A *ptr = new A;
    COMPARE(A::n_objects, 1);
    DEFER({ delete ptr; });
    COMPARE(A::n_objects, 1);
  }
  COMPARE(A::n_objects, 0);
  {
    A *ptr = new A[10];
    COMPARE(A::n_objects, 10);
    DEFER({ delete[] ptr; });
    COMPARE(A::n_objects, 10);
  }
  COMPARE(A::n_objects, 0);
}

TEST(defer_decrement) {
    int n = 2;
    {
      DEFER({ --n; });
      COMPARE(n, 2);
    }
    COMPARE(n, 1);
    {
      auto &&defer0 = ext::defer([&]() { --n; });
      COMPARE(n, 1);
    }
    COMPARE(n, 0);
}
