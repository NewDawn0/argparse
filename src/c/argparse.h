///////////////////////////////////////////////////
// Argparser - A simple argument parsing library //
// Version: c                                    //
// Copyright: NewDawn0 (Tom) 10.12.2022          //
// License: MIT                                  //
///////////////////////////////////////////////////

// header guard
#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

// libs
#include "vector.h"

// libs

//definitions
#define red "\033[1;31m"
#define reset "\033[0m"

// parser settings
typedef void (*fnRef)();
typedef struct ParserSettings {
    bool allowInvalidArgs = false;
    bool allowNoArgs = false;
    struct ParserSettingsEventFunctions {
        fnRef NoArgErr = NULL;
        fnRef MissingArgErr = NULL;
        fnRef InvalidArgErr = NULL;
        fnRef MultiArgErr = NULL;
        fnRef NoArgsAllowed = NULL;
    } eventFunctions;
} ParserSettings;
#undef fnRef

// header guard
#undef
