#include "cut.h"

#include <stdarg.h>
#include <string.h>

static FILE* s_pTestLogFile = NULL;
static bool s_bAllowLogColors = false;

#ifdef _WIN32
    #include <Windows.h>
    #include <direct.h>
    #define PATH_MAX MAX_PATH

    static DWORD s_dwDefaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    static HANDLE s_hConsole = NULL;
#else
    #include <unistd.h>
    #include <limits.h>
    #include <sys/stat.h>
#endif

void CutLogError(const CutTestContext* ctx) {
    printf("(%s:%i) Test case \"%s\" failed: %s\n",
        ctx->caller_file, ctx->caller_line, ctx->current_test->name, ctx->error_message
    );
    if (s_pTestLogFile != NULL) {
        fprintf(s_pTestLogFile, "(%s:%i) Test case \"%s\" failed: %s\n",
            ctx->caller_file, ctx->caller_line, ctx->current_test->name, ctx->error_message
        );
    }
}
void CutLogSuccess(const CutTestContext* ctx) {
    printf("Test case \"%s\" succeeded\n", ctx->current_test->name);
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

void CutParseArgs(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            puts("Flags:");
            puts(" * --color|-c ==> Use colors in printout");
            exit(0);
        } else if (strcmp(argv[i], "--colors") == 0 || strcmp(argv[i], "-c") == 0) {
            s_bAllowLogColors = true;
#ifdef _WIN32
            if (s_hConsole == NULL) {
                s_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
                GetConsoleScreenBufferInfo(s_hConsole, &consoleInfo);
                s_dwDefaultColor = consoleInfo.wAttributes;
            }
#endif
        }
    }
}
void CutOpenTestLogFile(const char* filename) {
#ifdef _WIN32
    fopen_s(&s_pTestLogFile, filename, "w");
#else
    s_pTestLogFile = fopen(filename, "w");
#endif
}
void CutCloseTestLogFile() {
    if (s_pTestLogFile != NULL) {
        fclose(s_pTestLogFile);
    }
}

void CutSetTextColorRed() {
    if (s_bAllowLogColors) {
#ifdef _WIN32
        if (s_hConsole != NULL) SetConsoleTextAttribute(s_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
        printf("\x1B[31;1m");
#endif
    }
}
void CutSetTextColorGreen() {
    if (s_bAllowLogColors) {
#ifdef _WIN32
        if (s_hConsole != NULL) SetConsoleTextAttribute(s_hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
        printf("\x1B[32;1m");
#endif
    }
}
void CutResetTextColor() {
    if (s_bAllowLogColors) {
#ifdef _WIN32
        if (s_hConsole != NULL) SetConsoleTextAttribute(s_hConsole, s_dwDefaultColor);
#else
        printf("\x1B[0m");
#endif
    }
}
bool CutLogColorsEnabled() {
    return s_bAllowLogColors;
}