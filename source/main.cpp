#include <core/definitions.h>
#include <core/hashtable.h>
#include <core/linklist.h>
#include <core/allocator.h>
#include <compiler/token.h>
#include <compiler/lexer.h>
#include <environment/runtime.h>
#include <platform/file.h>
#include <platform/system.h>

#if defined(_WIN32)
#   include <platform/win32/system.cpp>
#   include <platform/win32/file.cpp>

int
main(int argc, char **argv)
{

    environment_arguments args = {};
    args.count  = argc;
    args.list   = argv;

    b32 runtime_init_code = environment_runtime_init(&args);
    if (runtime_init_code != environment_return_codes::init_success)
        return -1;

    b32 runtime_main_code = environment_runtime_main();
    environment_runtime_exit(runtime_main_code);

}

#endif

#include <core/allocator.cpp>
#include <core/hashtable.cpp>
#include <environment/runtime.cpp>
#include <compiler/lexer.cpp>
