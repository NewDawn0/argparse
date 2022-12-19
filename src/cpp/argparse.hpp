///////////////////////////////////////////////////
// Argparser - A simple argument parsing library //
// Version: cpp                                  //
// Copyright: NewDawn0 (Tom) 10.12.2022          //
// License: MIT                                  //
///////////////////////////////////////////////////

// header guard
#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

// libs
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <iterator>
#include <iostream>

//definitions
#define red "\033[1;31m"
#define reset "\033[0m"

// ParserSettings
typedef void (*fnRef)();
typedef struct ParserSettings {
    bool allowInvalidArgs = false;
    bool allowNoArgs = false;
    struct ParserSettingsEventPhrases {
        std::string NoArgErr = "Provide at least one argument";
        std::string MissingArgErr = "Provide an argument for this option";
        std::string InvalidArgErr = "Invalid argument";
        std::string MultiArgErr = "This argument can only be provided once";
    } eventPhrases;
    struct ParserSettingsEventFunctions {
        fnRef NoArgErr = NULL;
        fnRef MissingArgErr = NULL;
        fnRef InvalidArgErr = NULL;
        fnRef MultiArgErr = NULL;
        fnRef NoArgsAllowed = NULL;
    } eventFunctions;
} ParserSettings;
#undef fnRef

// Argument Parser
class ArgParser {
    private:
        // vars
        ParserSettings parserSettings;
        std::vector<std::string> argKeys, multi, multiCmp, noNext;
        // functions
        void eprintln(std::string eventString, std::string flag = "");
        int contains(std::vector<std::string> targetVec, std::string item);
    public:
        // vars
        std::vector<std::string> otherArgs;
        std::map<std::string, std::vector<std::string>> args;
        // functions
        void config(ParserSettings newParserSettings);
        void parse(int argc, char *argv[]);
        void addArg(std::string arg, bool multipleAllowed = false, bool requireNextArg = true);
};

//// Private
// print events
inline void ArgParser::eprintln(std::string eventString, std::string flag) {
    std::cerr << red "ArgParse Error " << reset;
    if (flag.length() > 0) {std::cerr << red "\'" << reset << flag << red red "\' " << reset;}
    std::cerr << eventString << std::endl;
    exit(1);
}

// check if string is in vector
inline int ArgParser::contains(std::vector<std::string> targetVec, std::string item) {
    auto res = std::find(targetVec.begin(), targetVec.end(), item);
    if (res != targetVec.end()) {
        return res - targetVec.begin();
    } else {
        return -1;
    }
}

//// Public
// change defaults
inline void ArgParser::config(ParserSettings newParserSettings) {
    ArgParser::parserSettings = newParserSettings;
}
// add arguments
inline void ArgParser::addArg(std::string arg, bool multipleAllowed, bool requireNextArg) {
    if (multipleAllowed) {ArgParser::multi.push_back(arg);}
    if (!requireNextArg) {ArgParser::noNext.push_back(arg);}
    ArgParser::argKeys.push_back(arg);
}

// parse arguments
inline void ArgParser::parse(int argc, char *argv[]) {
    // if no args are found
    if (argc == 1) {
        if (parserSettings.allowNoArgs) {
            // fu event if configured else ignore
            if (parserSettings.eventFunctions.NoArgsAllowed != NULL) {
                parserSettings.eventFunctions.NoArgsAllowed();
            }
        } else {
            // fn event if configured else run default error
            if (parserSettings.eventFunctions.NoArgErr == NULL) {
                ArgParser::eprintln(parserSettings.eventPhrases.NoArgErr);
            }
            parserSettings.eventFunctions.NoArgErr();
        }
    // if there are arguments
    } else {
        // loop over argumetns except first
        int argIndex = 1;
        while (argIndex < argc) {
            // check if its actually an argument
            int argKeysIndex = ArgParser::contains(ArgParser::argKeys, argv[argIndex]);
            // invalid arg
            if (argKeysIndex == -1) {
                // fn event if configured else run default error
                if (parserSettings.allowInvalidArgs) {
                    otherArgs.push_back(argv[argIndex]);
                } else {
                    if (parserSettings.eventFunctions.InvalidArgErr == NULL) {
                        ArgParser::eprintln(parserSettings.eventPhrases.InvalidArgErr, argv[argIndex]);
                    }
                    parserSettings.eventFunctions.InvalidArgErr();
                }
            // valid arg
            } else {
                std::vector<std::string> tmp;
                // check if arg is allowed multiple times or hasn't occured yet
                if (ArgParser::contains(ArgParser::multi, argv[argIndex]) != -1 || ArgParser::contains(ArgParser::multiCmp, argv[argIndex]) == -1) {
                    // valid
                    ArgParser::multiCmp.push_back(argv[argIndex]);
                    // if no next arg is required
                    if (ArgParser::contains(ArgParser::noNext, argv[argIndex]) != -1) {
                        ArgParser::args[argv[argIndex]] = tmp;
                    // if next arg is required
                    } else {
                        // if there is a next arg
                        if (argc > argIndex+1) {
                            // create vector of args if not present
                            if (ArgParser::args.count(argv[argIndex])) {
                                ArgParser::args[argv[argIndex]].push_back(argv[argIndex+1]);
                            // create vec
                            } else {
                                std::vector<std::string> vec;
                                vec.push_back(argv[argIndex+1]);
                                ArgParser::args[argv[argIndex]] = vec;
                            }
                            argIndex++;
                        } else {
                            if (parserSettings.eventFunctions.MissingArgErr == NULL) {
                                ArgParser::eprintln(parserSettings.eventPhrases.MissingArgErr, argv[argIndex]);
                            }
                            parserSettings.eventFunctions.MissingArgErr();
                        }
                    }
                // invalid due to disallowed multiple times
                } else {
                    // fn event if configured else run default error
                    if (parserSettings.eventFunctions.MultiArgErr == NULL) {
                        ArgParser::eprintln(parserSettings.eventPhrases.MultiArgErr, argv[argIndex]);
                    }
                    parserSettings.eventFunctions.MultiArgErr();
                }
            }
            // Inc
            argIndex++;
        }
    }
}

// header guard
#endif
