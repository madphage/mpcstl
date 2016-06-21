#pragma once

// Need to have this one level of indirection so expansion happens of names before we concatenate them
#define CAT(x, y) CAT_INNER(x, y)
#define CAT_INNER(x, y) x ## y
#define CAT3(x, y, z) CAT3_INNER(x, y, z)
#define CAT3_INNER(x, y, z) x ## y ## z

typedef enum _MptStatus {
	MptSuccess = 0,
	MptFailure = -1,
	MptUnexpected = -2
} MptStatus;

#define RUNTIME_ASSERT(x) { \
	if (!(x)) { \
		goto Exit; \
	} \
}

#define NOTNULL(x) RUNTIME_ASSERT(x)

#define FAIL(status) { \
	res = status; \
	goto Exit; \
}

