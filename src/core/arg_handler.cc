//
// Created by Adithya Swaminathan on 10/17/20.
//

#include <core/arg_handler.h>
#include <core/probability_finder.h>

#include <fstream>

namespace naivebayes {

void ArgHandler::WriteProbabilitiesToFile(std::ofstream &ofstream, naivebayes::ProbabilityFinder &pf, naivebayes::Model &model, vector<double> prior_probabilities) {
  for(size_t cl = 0; cl <= 9; cl++) { //class number
    for(size_t x=0;x<28;x++) {
      for (size_t y = 0; y < 28; y++) {
        ofstream <<cl << " "<<x<< " "<<y<<" "<< prior_probabilities[cl] << " "
                 << pf.FindProbabilityOfShadingAtPoint(model, cl, std::make_pair(x, y)) << std::endl;
      }
    }
  }
}

void ArgHandler::RunCommandLineFunctions(const vector<string> &all_args, naivebayes::ProbabilityFinder &pf, naivebayes::Model &model) {
  if(all_args.empty()) {
    throw std::invalid_argument("Empty argument vector!");
  }
  //input and output streams
  std::ifstream ifstream;
  std::ofstream ofstream;

  //train TrainingLabelsName TrainingImagesName save FileToWriteTo
  if(all_args[0] == "train") {
    if(all_args.size() == 5) {
      //ifstream.open(all_args[1]); // mnistdatatraining/traininglabels
      vector<double> prior_probabilities = pf.CalculatePriorProbabilities(all_args[1],  model);

      //read through training images
      std::ifstream ifstream_images;
      ifstream_images.open(all_args[2]); // mnistdatatraining/trainingimages
      while(ifstream_images.good()) {
        ifstream_images>> model;
      }

      if (all_args[3] == "save") {
        //saves model to file
        ofstream.open(all_args[4]); // data/probabilities
        WriteProbabilitiesToFile(ofstream, pf, model, prior_probabilities);
      }

    } else std::cout<<"Invalid amount of command line args. Please try again."<<std::endl;
  } else if(all_args[0] == "load") {  //load FileToLoadUp
    if(all_args.size() == 2) {
      ifstream.clear();
      ifstream.open(all_args[1]);

      while(ifstream.good()) {
        string line;
        getline(ifstream, line);
        std::cout<<line<<std::endl;
      }

    } else std::cout<<"Invalid amount of command line args. Please try again."<<std::endl;
  } else {
    std::cout<<"Invalid command line arg. Please try again."<<std::endl;
  }
}

//https://stackoverflow.com/questions/15344714/convert-command-line-argument-to-string
void ArgHandler::ProcessCommandLineArgs(int argc, char *argv[], naivebayes::Model &model) {
  //vector of strings consisting of all args passed in (not counting file name)
  vector<string> all_args;
  naivebayes::ProbabilityFinder pf;
  if (argc > 1) {
    all_args.assign(argv + 1, argv + argc);
  }
  RunCommandLineFunctions(all_args, pf, model);
}

}