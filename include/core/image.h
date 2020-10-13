#include <string>
#include <vector>
#include <core/raster.h>

namespace naivebayes {

using std::vector;
using std::string;

/**
 * Stores all the data for all the images, including all the training dataset variables
 */
class Image {
 private:
  /**
   * Updates frequency_map_ variable with new pixels for a certain class
   * @param s The string representing the current line of the text file
   * @param image_class The class of the current Raster image
   */
  void UpdateFrequencyMap(const string& s, size_t image_class);

  /**
   * Adds current Raster image to raster_list_ variable
   * @param char_vec The current vector of characters in the current line of the text file
   * @param current_raster The current Raster image
   * @param image_class The class of the current Raster image
   */
  void AddCurrentRasterToList(const vector<char> &char_vec, naivebayes::Raster &current_raster, size_t image_class);

  /**
   * Initializes frequency_map_ variable to all zeros
   * @param frequency_map_size The desired size of the frequency map
   */
  void InitializeFrequencyMap(size_t frequency_map_size);

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
   * Sets value of training_label_vec_ variable to variable passed in
   * @param training_label_vec The vector of ints corresponding to all the training label numbers
   */
  void SetTrainingLabelVec(const vector<int> &training_label_vec);

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
   * Sets value of frequency_map_ variable to variable passed in
   * @param frequency_map The map tracking frequency of shading at each class
   */
  void SetFrequencyMap(const map<size_t, vector<vector<size_t>>> &frequency_map);

  /**
   * Gets the frequency_map_ variable
   * @return A map that tracks frequency of shading at each class
   */
  map<size_t, vector<vector<size_t>>> GetFrequencyMap() const;

  /**
   * Sets value of raster_list_ variable to variable passed in
   * @param raster_list The vector of Raster elements in a file
   */
  void SetRasterList(const vector<naivebayes::Raster> &raster_list);

  /**
   * Gets the list of Raster elements (a la images) in a file
   * @return Vector of Raster elements in a file
   */
  vector<naivebayes::Raster> GetRasterList() const;
};
}  // namespace naivebayes
