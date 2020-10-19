//
// Created by Adithya Swaminathan on 10/11/20.
//

#ifndef NAIVE_BAYES_IMAGE_H
#define NAIVE_BAYES_IMAGE_H

#include <iostream>
#include <vector>
#include <map>

namespace naivebayes {

using std::vector;
using std::map;

/**
 * Corresponds to a single image from the training data set, used to rasterize characters
 */
class Image {
 public:

  /**
   * Default Constructor of Image Class
   */
  Image();

  /**
   * Sets value of image_vector_ variable to variable passed in
   * @param image_vector A 2d-vector of characters corresponding to a single image
   */
  void SetImageVector(const vector<vector<char>> &image_vector);

  /**
   * Returns vector of all characters in image
   * @return Value of the image_vector_ variable
   */
  vector<vector<char>> GetImageVector() const;

  /**
   * Sets value of image_class_ variable to variable passed in
   * @param image_class A number to assign a class variable to an image
   */
  void SetImageClass(size_t image_class);

  /**
   * Returns class value of the Image instance
   * @return Value of the image_class_ variable
   */
  size_t GetImageClass() const;

  /**
   * Checks if a character of the image, given a certain position, is a space
   * @param r The row position of the character in image_vector_
   * @param c The column position of the character in image_vector_
   * @return Whether the character in image_vector_ at the specific position is a space
   */
  bool CheckSpace(size_t r, size_t c) const;
 private:
  //vector of characters in a single image
  vector<vector<char>> image_vector_;

  //class of given image
  size_t image_class_;
};
} //namespace naivebayes

#endif  // NAIVE_BAYES_IMAGE_H
