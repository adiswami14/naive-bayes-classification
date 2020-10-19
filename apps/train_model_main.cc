#include <core/arg_handler.h>

using std::vector;
using std::string;
using std::map;


int main(int argc, char* argv[]) {
  naivebayes::ArgHandler argHandler;
  naivebayes::Model model;
  argHandler.ProcessCommandLineArgs(argc, argv, model);
  //vector<double> p = argHandler.GetLoadedPriorProbabilities();
  //map<size_t, vector<vector<double>>> m = argHandler.GetLoadedFrequencyMap();
  return 0;
}
