//
// Created by Adithya Swaminathan on 10/17/20.
//

#include <core/arg_handler.h>
#include <core/probability_finder.h>

#include <fstream>

namespace naivebayes {

ArgHandler::ArgHandler() {
  image_size_= 28;
  InitializeLoadedFeatureProbMap(image_size_);
}

ArgHandler::ArgHandler(size_t image_size) {
  image_size_ = image_size;
  InitializeLoadedFeatureProbMap(image_size_);
}

vector<double> ArgHandler::GetLoadedPriorProbabilities() {
  return loaded_prior_probabilities_;
}

map<size_t, vector<vector<double>>> ArgHandler::GetLoadedFrequencyMap() {
  return loaded_feature_prob_map_;
}

void ArgHandler::WriteProbabilitiesToFile(std::ofstream &ofstream, naivebayes::ProbabilityFinder &pf, vector<double> prior_probabilities) {
  for(size_t cl = 0; cl <= 9; cl++) { //class number
    for(size_t x=0;x<image_size_;x++) {
      for (size_t y = 0; y < image_size_; y++) {
        ofstream <<cl << " "<<x<< " "<<y<<" "<< prior_probabilities[cl] << " "
                 << pf.FindProbabilityOfShadingAtPoint(cl, std::make_pair(x, y)) << std::endl;
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
      model.ReadLabels(all_args[1]);

      //read through training images
      std::ifstream ifstream_images;
      ifstream_images.open(all_args[2]); // mnistdatatraining/trainingimages
      while(ifstream_images.good()) {
        ifstream_images>> model;
      }
      pf.SetModel(model);
      vector<double> prior_probabilities = pf.CalculatePriorProbabilities();
      if (all_args[3] == "save") {
        //saves model to file
        ofstream.open(all_args[4]); // data/probabilities
        WriteProbabilitiesToFile(ofstream, pf,  prior_probabilities);
      }

    } else throw std::invalid_argument("Invalid amount of command line args. Please try again.");
  } else if(all_args[0] == "load") {  //load FileToLoadUp
    if(all_args.size() == 2) {
      ifstream.clear();
      ifstream.open(all_args[1]);
      int curr_class = 1;
      while(ifstream.good()) {
        size_t cl;
        size_t x;
        size_t y;
        double prior_prob_for_class;
        double feature_prob_at_point;
        ifstream >> cl >> x >> y >> prior_prob_for_class >> feature_prob_at_point;
        if(curr_class!=cl) {
          loaded_prior_probabilities_.push_back(prior_prob_for_class);
        }
        curr_class= cl;
        loaded_feature_prob_map_[cl][x][y] = feature_prob_at_point;
      }

    } else throw std::invalid_argument("Invalid amount of command line args. Please try again.");
  } else {
    throw std::invalid_argument("Invalid command line arg. Please try again.");
  }
}

//https://stackoverflow.com/questions/15344714/convert-command-line-argument-to-string
void ArgHandler::ProcessCommandLineArgs(int argc, char *argv[], naivebayes::Model &model) {
  //vector of strings consisting of all args passed in (not counting file name)
  vector<string> all_args;
  naivebayes::ProbabilityFinder pf(model);
  if (argc > 1) {
    all_args.assign(argv + 1, argv + argc);
  }
  RunCommandLineFunctions(all_args, pf, model);
}

void ArgHandler::InitializeLoadedFeatureProbMap(size_t frequency_map_size) {
  vector<vector<double>> vec_big;
  vector<double> vec_small;

  for(size_t cl = 0; cl <= 9; cl++) {
    for (size_t x = 0; x < frequency_map_size; x++) {
      for (size_t y = 0; y < frequency_map_size; y++) {
        vec_small = vector<double>(frequency_map_size, 0.0);
      }
      vec_big.push_back(vec_small);
    }
    loaded_feature_prob_map_[cl] = vec_big;
    vec_big.clear();
  }

}

}