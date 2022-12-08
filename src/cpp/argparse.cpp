///////////////////////////////////////////////////
// argparse - A simple cpp argument parser       //
// Written/Copyright by NewDawn0 (Tom) 7.12.2022 //
// Code is licensed under the MIT license        //
///////////////////////////////////////////////////

// libs
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include "argparse.hpp"

//definitions
#define red "\033[1;31m"
#define reset "\033[0m"

// use
using std::pair;
using std::vector;
using std::string;

// parser contains
int libargparse::ArgParser::contains(vector<string> targetVec, string item) {
    auto res = std::find(targetVec.begin(), targetVec.end(), item);
    if (res != targetVec.end()) {
        return res - targetVec.begin();
    } else {
        return -1; 
    }
}
// parser order arguments in argtainer
void libargparse::ArgParser::order() {
    // create map element for every argument
    for (string &item : argKeys) {
        vector<string> elems;
        argtainer[item] = elems;
    }
    // add items to argument map
    for (pair<string, string> &item : container) {
        argtainer[std::get<0>(item)].push_back(std::get<1>(item));
    }
    // seach for unused args and dupliate options
    vector<string> del, change;
    for (const auto &item : argtainer) {
        if (item.second.empty()) {
            del.push_back(item.first);
        } else if (item.second[0].empty()) {
            change.push_back(item.first);
        }
    }
    // remove unused options
    for (string &item : del) {
        argtainer.erase(item);
    }
    // remove duplicates of single arg options
    for (string &item : change) {
        argtainer[item].resize(1);
    }
}

// parser addArg
void libargparse::ArgParser::addArg(string arg, bool reqNextArg, bool multipleAllowed) {
    if (multipleAllowed) libargparse::ArgParser::multi.push_back(arg);
    if (!reqNextArg) libargparse::ArgParser::noNextReq.push_back(arg);
    libargparse::ArgParser::argKeys.push_back(arg);
}

// parser parse
void libargparse::ArgParser::parse(int argc, char *argv[], bool allowInvalidArguments, bool allowNoArguments) {
    if (argc == 1 && !allowNoArguments) {
        // Override default
        if (libargparse::ArgParser::events["NoArguments"] != NULL) {
            events["NoArguments"]();
        } else {
            // err out if no argument is listed and allow no arguments is listed as false
            std::cerr << red << "ArgParse Error "<< reset << ArgParserNoArgErr << std::endl;
            exit(1);
        }
    } else if (argc == 1) {
        // Override default
        if (libargparse::ArgParser::events["AllowNoArguments"] != NULL) {
            events["AllowNoArguments"]();
        }
    } else {
        int argIndex = 1;
        while (argIndex < argc) {
            if (contains(libargparse::ArgParser::argKeys, argv[argIndex]) != -1) {
                if (contains(libargparse::ArgParser::multi, argv[argIndex]) != -1 || contains(libargparse::ArgParser::multiDone, argv[argIndex]) == -1) {
                    multiDone.push_back(argv[argIndex]);
                    // create temporary container
                    pair<string, string> pContainer;
                    // check if argument needs next arg
                    if (contains(libargparse::ArgParser::noNextReq, argv[argIndex]) != -1) {
                        // add arg to container
                        pContainer = std::make_pair(argv[argIndex], "");
                        container.push_back(pContainer);
                    } else {
                        // look if there is next arg
                        if (argc-1 >= argIndex+1) {
                            // add arg to container
                            pContainer = std::make_pair(argv[argIndex], argv[argIndex+1]);
                            container.push_back(pContainer);
                            argIndex++;
                        } else {
                            // Override default
                            if (libargparse::ArgParser::events["MissingArg"] != NULL) {
                                events["MissingArg"]();
                            } else {
                                // err if no next arg is found
                                std::cerr << red << "ArgParse Error " << reset << ArgParserMissingArgErr << std::endl;
                                exit(1);
                            }
                        }
                    }
                } else {
                    // Override default
                    if (libargparse::ArgParser::events["MultiArg"] != NULL) {
                        events["MultiArg"]();
                    } else {
                        // err if there are multiple of the same arg which is not allowed as default
                        std::cerr << red << "ArgParse Error " << reset << ArgParserMultiArgErr << red << " \'" << reset << argv[argIndex] << red << "\'" << reset << std::endl; 
                        exit(1);
                    }
                }
            } else {
                // err if invalid arguments aren't allowed
                if (!allowInvalidArguments) {
                    // Override default
                    if (libargparse::ArgParser::events["InvalidArg"] != NULL) {
                        events["InvalidArg"]();
                    } else {
                        std::cerr << red << "ArgParse Error " << reset << ArgParserInvalidArgErr <<red << " \'" << reset << argv[argIndex] << red << "\'" << reset << std::endl;
                        exit(1);
                    }
                }
                // collect "invalid" argument as otherArgs
                otherArgs.push_back(argv[argIndex]);
            }
            argIndex++;
        }
    }
    libargparse::ArgParser::order();
}

// addTrigger - Override default behaviour
void libargparse::ArgParser::addTrigger(string event, void (*fnPtr)()) {
    // if event exist overwrite original behaviour
    if (contains(libargparse::ArgParser::eventTriggers, event) != -1) {
        events[event] = fnPtr;
    }
}
