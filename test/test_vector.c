#include "minunit.h"
#include "stdint.h"
#include "stdio.h"

// for minunit.h
int tests_run = 0;

typedef struct _BasicStructItem {
	int a;
	int b;
	int c;
} BasicStructItem;

// .mpt is a MadPhage Template file, a header intended to be included
// multiple times
// the default naming scheme for a vector included this way is
// V<itemname>, e.g. in this case VBasicStructItem
//#define VectorITEM BasicStructItem
//#include "vector.mpt"

//#define VectorITEM uint32_t
//#define VectorITEM_primitive 1
//#include "vector.mpt"

static char * test_fail() {
	MU_ASSERT(1, "didn't fail as intended");
	MU_ASSERT(0, "failed as intended");
	return 0;
};

/*
static char * test_vector_alloc() {
	size_t i = 0;
	Vuint32_t * uint32_vector = NULL;

	uint32_vector = Vuint32_t_mk_prealloc(4);
	MU_ASSERT(uint32_vector, "mk_prealloc(4)");

	for (i = 0; i < 4; ++i) {
		MU_ASSERT(uint32_vector->data[i] == 0, "data should be initialized to zero");
	}
	return 0;
}
*/

static char * all_tests() {
	MU_RUN_TEST(test_fail);
	//MU_RUN_TEST(test_vector_alloc);
	return 0;
}

int main(int argc, char *argv[]) {
	char * result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	} else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);

	return result != 0;
}
