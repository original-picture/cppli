#pragma once

#include "command_macros.h"
#include "configuration.h"
#include "documentation.h"
#include "run.h"

#ifdef CPPLI_IMPLEMENTATION
    #include "configuration.cpp"
    #include "arg_parsing.cpp"
    #include "documentation.cpp"
    #include "subcommand.cpp"
    #include "user_error.cpp"
    #include "cppli.cpp"
    #include "command_registration.cpp"
#endif
