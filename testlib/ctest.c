#include "ctest.h"

//#define COLOR_OK

static size_t ctest_errorsize;
static char* ctest_errormsg;
#define MSG_SIZE 4096
static char ctest_errorbuffer[MSG_SIZE];
static jmp_buf ctest_err;
static int color_output = 1;
static const char* suite_name;

typedef int (*filter_func)(struct ctest*);

#define ANSI_BLACK    "\033[0;30m"
#define ANSI_RED      "\033[0;31m"
#define ANSI_GREEN    "\033[0;32m"
#define ANSI_YELLOW   "\033[0;33m"
#define ANSI_BLUE     "\033[0;34m"
#define ANSI_MAGENTA  "\033[0;35m"
#define ANSI_CYAN     "\033[0;36m"
#define ANSI_GREY     "\033[0;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED     "\033[01;31m"
#define ANSI_BGREEN   "\033[01;32m"
#define ANSI_BYELLOW  "\033[01;33m"
#define ANSI_BBLUE    "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN    "\033[01;36m"
#define ANSI_WHITE    "\033[01;37m"
#define ANSI_NORMAL   "\033[0m"

CTEST(ctest_dummy, nop1) { }
CTEST(ctest_dummy, nop2) { }

static void msg_start(const char* color, const char* title) {
    int size;
    if (color_output) {
        size = snprintf(ctest_errormsg, ctest_errorsize, "%s", color);
        ctest_errorsize -= size;
        ctest_errormsg += size;
    }
    size = snprintf(ctest_errormsg, ctest_errorsize, "  %s: ", title);
    ctest_errorsize -= size;
    ctest_errormsg += size;
}

static void msg_end(void) {
    int size;
    if (color_output) {
        size = snprintf(ctest_errormsg, ctest_errorsize, ANSI_NORMAL);
        ctest_errorsize -= size;
        ctest_errormsg += size;
    }
    size = snprintf(ctest_errormsg, ctest_errorsize, "\n");
    ctest_errorsize -= size;
    ctest_errormsg += size;
}

void CTEST_LOG(char *fmt, ...)
{
    va_list argp;
    msg_start(ANSI_BLUE, "LOG");

    va_start(argp, fmt);
    int size = vsnprintf(ctest_errormsg, ctest_errorsize, fmt, argp);
    ctest_errorsize -= size;
    ctest_errormsg += size;
    va_end(argp);

    msg_end();
}

void CTEST_ERR(char *fmt, ...)
{
    va_list argp;
    msg_start(ANSI_YELLOW, "ERR");

    va_start(argp, fmt);
    int size = vsnprintf(ctest_errormsg, ctest_errorsize, fmt, argp);
    ctest_errorsize -= size;
    ctest_errormsg += size;
    va_end(argp);

    msg_end();
}

void assert_str(const char* exp, const char*  real, const char* caller, int line) {
    if (strcmp(exp, real) != 0) {
        CTEST_ERR("%s:%d  expected '%s', got '%s'", caller, line, exp, real);
        longjmp(ctest_err, 1);
    }
}

void assert_equal(int exp, int real, const char* caller, int line) {
    if (exp != real) {
        CTEST_ERR("%s:%d  expected %d, got %d", caller, line, exp, real);
        longjmp(ctest_err, 1);
    }
}

void assert_not_equal(int exp, int real, const char* caller, int line) {
    if ((exp) == (real)) {
        CTEST_ERR("%s:%d  should not be %d", caller, line, real);
        longjmp(ctest_err, 1);
    }
}

void assert_null(void* real, const char* caller, int line) {
    if ((real) != NULL) {
        CTEST_ERR("%s:%d  should be NULL", caller, line);
        longjmp(ctest_err, 1);
    }
}

void assert_not_null(void* real, const char* caller, int line) {
    if (real == NULL) {
        CTEST_ERR("%s:%d  should not be NULL", caller, line);
        longjmp(ctest_err, 1);
    }
}

void assert_true(int real, const char* caller, int line) {
    if ((real) == 0) {
        CTEST_ERR("%s:%d  should be true", caller, line);
        longjmp(ctest_err, 1);
    }
}

void assert_false(int real, const char* caller, int line) {
    if ((real) != 0) {
        CTEST_ERR("%s:%d  should be false", caller, line);
        longjmp(ctest_err, 1);
    }
}

void assert_fail(const char* caller, int line) {
    CTEST_ERR("%s:%d  shouldn't come here", caller, line);
    longjmp(ctest_err, 1);
}


static int suite_all(struct ctest* t) {
    return 1;
}

static int suite_filter(struct ctest* t) { 
    return strncmp(suite_name, t->ssname, strlen(suite_name)) == 0;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
static uint64_t getCurrentTime(void) {
    return 0;
}
#else
static uint64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, NULL);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_usec);
    return now64;
}
#endif

static void color_print(const char* color, const char* text) {
    if (color_output)
        printf("%s%s"ANSI_NORMAL"\n", color, text);
    else
        printf("%s\n", text);
}

#ifdef __APPLE__
static void *find_symbol(struct ctest *test, const char *fname)
{
    size_t len = strlen(test->ssname) + 1 + strlen(fname);
    char *symbol_name = (char *) malloc(len + 1);
    memset(symbol_name, 0, len + 1);
    snprintf(symbol_name, len + 1, "%s_%s", test->ssname, fname);

    //fprintf(stderr, ">>>> dlsym: loading %s\n", symbol_name);
    void *symbol = dlsym(RTLD_DEFAULT, symbol_name);
    if (!symbol) {
        //fprintf(stderr, ">>>> ERROR: %s\n", dlerror());
    }
    // returns NULL on error
    
    free(symbol_name);
    return symbol;
}
#endif

int main(int argc, const char *argv[])
{
    static int total = 0;
    static int num_ok = 0;
    static int num_fail = 0;
    static int num_skip = 0;
    static int index = 1;
    static filter_func filter = suite_all;

    if (argc == 2) {
        suite_name = argv[1];
        filter = suite_filter;
    }

    color_output = isatty(1);
    uint64_t t1 = getCurrentTime();

    struct ctest* ctest_begin = &__TNAME(ctest_dummy, nop1);
    struct ctest* ctest_end = &__TNAME(ctest_dummy, nop2);
    size_t stride = ((uint8_t *)&__TNAME(ctest_dummy, nop2)) - ((uint8_t *)&__TNAME(ctest_dummy, nop1));
#define CTEST_DEC_STRIDE(p) ((struct ctest*)(((uint8_t*)p) - stride))
#define CTEST_INC_STRIDE(p) ((struct ctest*)(((uint8_t*)p) + stride))

    // printf("Begin: %p, End: %p, Stride: %d\n", ctest_begin, ctest_end, stride);
    // find begin and end of section by comparing magics
    while (1) {
        struct ctest* t = CTEST_DEC_STRIDE(ctest_begin);
        if (t->magic != __CTEST_MAGIC) {
            break;
        }
        ctest_begin = CTEST_DEC_STRIDE(ctest_begin);
    }
    while (1) {
        struct ctest* t = CTEST_INC_STRIDE(ctest_end);
        if (t->magic != __CTEST_MAGIC) break;
        ctest_end = CTEST_INC_STRIDE(ctest_end);
    }
    ctest_end = CTEST_INC_STRIDE(ctest_end);    // end after last one
    // printf("# Tests: %d\n", (((uint8_t*)ctest_end) - ((uint8_t*)ctest_begin))/stride);
    // printf("Sizeof ctest: %d\n", sizeof(struct ctest));

    static struct ctest* test;
    for (test = ctest_begin; test != ctest_end; test = CTEST_INC_STRIDE(test)) {
        if (test == &__TNAME(ctest_dummy, nop1) || test == &__TNAME(ctest_dummy, nop2)) continue;
        if (filter(test)) total++;
    }

    for (test = ctest_begin; test != ctest_end; test = CTEST_INC_STRIDE(test)) {
        if (test == &__TNAME(ctest_dummy, nop1) || test == &__TNAME(ctest_dummy, nop2)) continue;
        if (filter(test)) {
            ctest_errorbuffer[0] = 0;
            ctest_errorsize = MSG_SIZE-1;
            ctest_errormsg = ctest_errorbuffer;
            printf("TEST %d/%d %s:%s ", index, total, test->ssname, test->ttname);
            fflush(stdout);
            if (test->skip) {
                color_print(ANSI_BYELLOW, "[SKIPPED]");
                num_skip++;
            } else {
                int result = setjmp(ctest_err);
                if (result == 0) {
/*
#ifdef __APPLE__
                    if (!test->setup) {
                        test->setup = find_symbol(test, "setup");
                    }
                    if (!test->teardown) {
                        test->teardown = find_symbol(test, "teardown");
                    }
#endif
*/

/*
                    if (test->setup) test->setup(test->data);
                    if (test->data) 
                      test->run(test->data);
                    else 
*/
                      test->run();
                    //if (test->teardown) test->teardown(test->data);
                    // if we got here it's ok
#ifdef COLOR_OK
                    color_print(ANSI_BGREEN, "[OK]");
#else
                    printf("[OK]\n");
#endif
                    num_ok++;
                } else {
                    color_print(ANSI_BRED, "[FAIL]");
                    num_fail++;
                }
                if (ctest_errorsize != MSG_SIZE-1) printf("%s", ctest_errorbuffer);
            }
            index++;
        }
    }
    uint64_t t2 = getCurrentTime();

    const char* color = (num_fail) ? ANSI_BRED : ANSI_GREEN;
    char results[80];
    snprintf(results, 80, "RESULTS: %d tests (%d ok, %d failed, %d skipped) ran in %lld ms", total, num_ok, num_fail, num_skip, (t2 - t1)/1000);
    color_print(color, results);
    return num_fail;
}
