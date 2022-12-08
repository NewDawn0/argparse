///////////////////////////////////////////////////
// argparse - A simple cpp argument parser       //
// Written/Copyright by NewDawn0 (Tom) 7.12.2022 //
// Code is licensed under the MIT license        //
///////////////////////////////////////////////////

// include guard
#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

// libs
#include <map>
#include <string>
#include <vector>
#include <utility>

// use
using std::map;
using std::pair;
using std::string;
using std::vector;

// definitions
#define ARGPARSER_NO_ARG_ERR "Provide at least one argument"
#define ARGPARSER_MISSING_ARG_ERR "Provide an argument for this option"
#define ARGPARSER_INVALID_ARG_ERR "Invalid argument"
#define ARGPARSER_MULTI_ARG_ERR "This Argument can only be provided once"

// argument parser
namespace libargparse {
    class ArgParser {
        private:
            // vars
            vector<string> argKeys, multi, multiDone, noNextReq;
            vector<string> eventTriggers = {"NoArgsErr", "AllowNoArgs", "MissingArgErr", "MultiArgErr", "InvalidArgErr"};
            map<string, void (*)()> events = {{"NoArgsErr", NULL},
                {"AllowNoArgs", NULL}, {"MissingArgErr", NULL}, 
                {"MultiArgErr", NULL}, {"InvalidArgErr", NULL}
            };
            // fn
            vector<pair<string, string>> container;
            int contains(vector<string> targetVec, string item);
            void order();
        public:
            // vars
            vector<string> otherArgs;
            map<string, vector<string>> argtainer;
            // fn
            void addArg(string arg, bool reqNextArg = true, bool multipleAllowed = false);
            void parse(int argc, char *argv[], bool allowInvalidArguments = false, bool allowNoArguments = false);
            void overrideEvent(string event, void (*fnPtr)());
            void help();
    };
}

// include guard
#endif
