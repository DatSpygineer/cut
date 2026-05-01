#include "cut.h"

#include <stdarg.h>
#include <string.h>

static FILE* s_pTestLogFile = NULL;

#ifdef _WIN32
    #include <Windows.h>
    #include <direct.h>
    #define PATH_MAX MAX_PATH
#else
    #include <unistd.h>
    #include <limits.h>
    #include <sys/stat.h>
#endif

void CutLogError(const CutTestContext* ctx) {
    printf("\x1B[31;1m(%s:%i) Test case \"%s\" failed: %s\x1B[0m\n",
        ctx->caller_file, ctx->caller_line, ctx->current_test->name, ctx->error_message
    );
    if (s_pTestLogFile != NULL) {
        fprintf(s_pTestLogFile, "(%s:%i) Test case \"%s\" failed: %s\n",
            ctx->caller_file, ctx->caller_line, ctx->current_test->name, ctx->error_message
        );
    }
}
void CutLogSuccess(const CutTestContext* ctx) {
    printf("\x1B[32;1mTest case \"%s\" succeeded\x1B[0m\n", ctx->current_test->name);
    if (s_pTestLogFile != NULL) {
        fprintf(s_pTestLogFile, "Test case \"%s\" succeeded\n", ctx->current_test->name);
    }
}

void CutPrintF(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    if (s_pTestLogFile != NULL) {
        va_start(args, format);
        vfprintf(s_pTestLogFile, format, args);
        va_end(args);
    }
}

void CutOpenTestLogFile(const char* filename) {
#ifdef _WIN32
    char* p = strrchr(filename, '\\');
#else
    char* p = strrchr(filename, '/');
#endif
    if (p != NULL) {
        char root[PATH_MAX] = { 0 };
        strncpy(root, filename, p - filename);
        root[p - filename] = '\0';
#ifdef _WIN32
        DWORD attrib = GetFileAttributes(root);
        if (attrib == INVALID_FILE_ATTRIBUTES || (attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            _mkdir(root);
        }
#else
        struct stat st = { 0 };
        if (stat(root, &st) != 0) {
            mkdir(root, 0777);
        }
#endif
    }

    s_pTestLogFile = fopen(filename, "w");
}
void CutCloseTestLogFile() {
    if (s_pTestLogFile != NULL) {
        fclose(s_pTestLogFile);
    }
}