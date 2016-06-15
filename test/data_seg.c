#include "ctest.h"

CTEST(foo, bar) {
	ASSERT_STR("foo", "foo");
	ASSERT_EQUAL(1, 2);
}
