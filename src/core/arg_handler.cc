//
// Created by Adithya Swaminathan on 10/17/20.
//

#include <core/arg_handler.h>

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

map<size_t, vector<vector<double>>> ArgHandler::GetLoadedFeatureProbMap() {
  return loaded_feature_prob_map_;
}

void ArgHandler::WriteProbabilitiesToFile(std::ofstream &ofstream, naivebayes::Model &model, const vector<double> &prior_probabilities) {
  for(size_t cl = 0; cl <= 9; cl++) { //class number
    for(size_t x=0;x<image_size_;x++) {
      for (size_t y = 0; y < image_size_; y++) {
        ofstream <<cl << " "<<x<< " "<<y<<" "<< prior_probabilities[cl] << " "
                 << model.FindProbabilityOfShadingAtPoint(cl, std::make_pair(x, y)) << std::endl;
      }
    }
  }
}

void ArgHandler::RunCommandLineFunctions(const vector<string> &all_args, naivebayes::Model &model) {
  if(all_args.empty()) {
    throw std::invalid_argument("Empty argument vector!");
  }
  //input and output streams
  std::ifstream ifstream;
  std::ofstream ofstream;

  if(all_args[0] == "train") {
    //trains model
    model.ReadLabels(all_args[1]);
    std::ifstream ifstream_images;
    ifstream_images.open(all_args[2]);
    while(ifstream_images.good()) {
      ifstream_images>> model;
    }

    vector<double> prior_probabilities = model.GetPriorProbabilities();
    if(all_args.size() == 5) {
      if (all_args[3] == "save") { //train TrainingLabelsName TrainingImagesName save FileToWriteTo
        //saves model to file
        SaveToFile(ofstream, all_args[4], model, prior_probabilities);
      }
    } else if(all_args.size()==6) {
      if(all_args[3] == "test") {
        //train TrainingLabelsName TrainingImagesName test TestLabelsName TestImagesName
        loaded_feature_prob_map_ = model.GetFeatureProbMap();
        TestModel(all_args[4], all_args[5], loaded_feature_prob_map_);
      }

    } else throw std::invalid_argument("Invalid amount of command line args. Please try again.");
  } else if(all_args[0] == "load") {
    //load FileToLoadUp test TestLabelFile TestImageFile
    if(all_args.size() == 5) {
      LoadFile(all_args[1]);
      if(all_args[2] == "test") {
        TestModel(all_args[3], all_args[4], loaded_feature_prob_map_);
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
  if (argc > 1) {
    all_args.assign(argv + 1, argv + argc);
  }
  RunCommandLineFunctions(all_args, model);
}

void ArgHandler::TestModel(const string& test_label_file, const string& test_image_file, const std::map<size_t, vector<vector<double>>> &map) const{
  naivebayes::Model m(map);
  m.ReadLabels(test_label_file);
  std::ifstream test;
  test.open(test_image_file);
  while(test.good()) {
    test>>m;
  }
  size_t matches = 0;
  vector<size_t> v = m.GetBestClassList();
  for(int x=0;x<v.size();x++) {
    if(v.at(x) == m.GetTrainingLabelVec().at(x))
      matches++;
  }
  std::cout<<"Your test images have been classified correctly "
            <<((double)matches/v.size())*100.0<<"% of the time!"<<std::endl;
}

void ArgHandler::LoadFile(const string &filename) {
  std::ifstream load_file_stream;
  load_file_stream.open(filename);
  int curr_class = 1;
  while(load_file_stream.good()) {
    size_t cl;
    size_t x;
    size_t y;
    double prior_prob_for_class;
    double feature_prob_at_point;
    load_file_stream >> cl >> x >> y >> prior_prob_for_class >> feature_prob_at_point;
    if(curr_class!=cl) {
      loaded_prior_probabilities_.push_back(prior_prob_for_class);
    }
    curr_class= cl;
    loaded_feature_prob_map_[cl][x][y] = feature_prob_at_point;
  }
}

void ArgHandler::SaveToFile(std::ofstream &ofstream, const string &filename, Model &model, const vector<double> &prior_probabilities) {
  ofstream.open(filename); // data/probabilities
  WriteProbabilitiesToFile(ofstream,model, prior_probabilities);
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