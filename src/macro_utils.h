#pragma once

#define CAT(x, y) CAT_INNER(x, y)
#define CAT_INNER(x, y) x ## y
#define CAT3(x, y, z) CAT3_INNER(x, y, z)
#define CAT3_INNER(x, y, z) x ## y ## z
