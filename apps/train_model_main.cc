#include <core/arg_handler.h>

using std::string;
using std::string;



int main(int argc, char* argv[]) {
  naivebayes::ArgHandler argHandler;
  naivebayes::Model model;
  argHandler.ProcessCommandLineArgs(argc, argv, model, 28);
  return 0;
}
