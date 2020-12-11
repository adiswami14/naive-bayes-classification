#include <core/arg_handler.h>

using std::vector;
using std::string;
using std::map;


int main(int argc, char* argv[]) {
  naivebayes::ArgHandler argHandler;
  naivebayes::Model model;
  argHandler.ProcessCommandLineArgs(argc, argv, model);
  return 0;
}
