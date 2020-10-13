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

/**
 * Corresponds to a single image from the training data set, used to rasterize characters
 */
class Raster {
 private:
  //vector of a single image
  vector<vector<char>> raster_vector_;

  //class of given image
  size_t raster_image_class_;
 public:

  /**
   * Default Constructor of Raster Class
   */
  Raster();

  /**
   * Sets value of raster_vector_ variable to variable passed in
   * @param raster_vector A 2d-vector of characters corresponding to a single image
   */
  void SetRasterVector(const vector<vector<char>> &raster_vector);

  /**
   * Sets value of raster_image_class variable to variable passed in
   * @param raster_image_class A number to assign a class variable to an image
   */
  void SetRasterImageClass(size_t raster_image_class);
};
} //namespace naivebayes

#endif  // NAIVE_BAYES_RASTER_H
