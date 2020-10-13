#include <string>
#include <vector>
#include <core/raster.h>

namespace naivebayes {

using std::vector;

/**
 * Stores all the data for all the images, including all the training dataset variables
 */
class Image {
 private:
  //vector that stores all the training labels
  vector<int> training_label_vec_;

  //2d vector that stores all images in training dataset
  vector<vector<char>> training_image_vec_;

  //vector of all Rasters (each raster corresponds to an image)
  vector<naivebayes::Raster> raster_list_;

  //map that stores a 2d-vector of how many times a certain pixel is shaded corresponding to each class of image
  map<size_t, vector<vector<size_t>>> frequency_map_;
 public:
  //The Laplacian constant for training the model
  const size_t kLaplaceSmoothingFactor = 1;

  /**
   * Default constructor of the Image class
   */
  Image();

  /**
   * Overloaded constructor of the Image class
   * @param frequency_map_size The desired size of the frequency_map_ variable
   */
  Image(size_t frequency_map_size);

  std::string GetBestClass() const;

  /**
   * Overloads the >> operator for a given image
   * @param istream The input stream given
   * @param image The instance of this Image class
   * @return The input stream after operator overload
   */
  friend std::istream &operator >>(std::istream &istream, Image& image);

  /**
   * Returns the training_label_vec variable
   * @return Private variable training_label_vec
   */
  vector<int> GetTrainingLabelVec() const;

  /**
   * Identifies how many training images are in a given class
   * @param class_num The number of the class
   * @return The number of training images in a certain class
   */
  size_t GetNumOfImagesInClass(size_t class_num) const;

  /**
   * Initializes frequency_map_ variable to all zeros
   * @param frequency_map_size The desired size of the frequency map
   */
  void InitializeFrequencyMap(size_t frequency_map_size);

  /**
   * Gets the frequency_map_ variable
   * @return A map that tracks frequency of shading at each class
   */
  map<size_t, vector<vector<size_t>>> GetFrequencyMap() const;
};
}  // namespace naivebayes

/*
TODO: rename this file. You'll also need to modify CMakeLists.txt.

You can (and should) create more classes and files in include/core (header
 files) and src/core (source files); this project is too big to only have a
 single class.

Make sure to add any files that you create to CMakeLists.txt.

TODO Delete this comment before submitting your code.
*/
