//
// Created by Adithya Swaminathan on 10/11/20.
//

#ifndef NAIVE_BAYES_RASTER_H
#define NAIVE_BAYES_RASTER_H

#include <iostream>
#include <vector>
#include <map>

namespace naivebayes {

using std::vector;
using std::map;

class Raster {
 private:
  //vector of a single image
  vector<vector<char>> raster_vector_;

  //class of given image
  size_t raster_image_class_;

  //stores whether certain pixel of a image of a certain class is shaded
  map<std::pair<size_t, size_t>, size_t> shade_map_;

 public:
  Raster();

  void SetRasterVector(const vector<vector<char>> &raster_vector);

  void SetRasterImageClass(size_t raster_image_class);
};
} //namespace naivebayes

#endif  // NAIVE_BAYES_RASTER_H
