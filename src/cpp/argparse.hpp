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
#define ArgParserNoArgErr "Provide at least one argument"
#define ArgParserMissingArgErr "Provide an argument for this option"
#define ArgParserInvalidArgErr "Invalid argument"
#define ArgParserMultiArgErr "This Argument can only be provided once"

// argument parser
namespace libargparse {
    class ArgParser {
        private:
            // vars
            vector<string> argKeys, multi, multiDone, noNextReq;
            vector<string> eventTriggers = {"NoArguments", "AllowNoArguments", "MissingArg", "MultiArg", "InvalidArg"};
            map<string, void (*)()> events = {{"NoArguments", NULL},
                {"AllowNoArguments", NULL}, {"MissingArg", NULL}, 
                {"MultiArg", NULL}, {"InvalidArg", NULL}
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
            void addTrigger(string event, void (*fnPtr)());
            void help();
    };
}

// include guard
#endif
