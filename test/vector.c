#include "ctest.h"

#include "stdint.h"
#include "stdio.h"

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

// Approach to .mpt is to have type.mpt have parameters TypeTHING TypeTHING_subattr
// where we have as many defaults as possible
// additionally, we have mptDeclare defaulting to 1 and mptDefine defaulting to 0,
// where both can be 1 at the same time if we want the definition to follow the declaration
// everything unused will be undefined at the end of the

#define VectorITEM uint32_t
#define VectorITEM_primitive 1
#define mptDefine 1
#include "vector.mpt"

CTEST(vector, alloc) {
	size_t i = 0;
	Vuint32_t * uint32_vector = NULL;

	uint32_vector = Vuint32_t_mk_prealloc(4);
	ASSERT_TRUE(uint32_vector != NULL);
	ASSERT_TRUE(uint32_vector->available == 4);
	ASSERT_TRUE(uint32_vector->used == 0);

	for (i = 0; i < 4; ++i) {
		ASSERT_TRUE(uint32_vector->items[i] == 0);
	}
}
