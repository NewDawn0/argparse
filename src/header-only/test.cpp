#include "argparse.hpp"
int main(int argc, char *argv[]) {
    ArgParser parser;
    parser.addArg("-h", true, false);
    parser.addArg("-i", true);
    parser.addArg("-o");
    parser.parse(argc, argv);
    for (std::string &arg:parser.otherArgs) {
        std::cout << arg << std::endl;
    }
}
