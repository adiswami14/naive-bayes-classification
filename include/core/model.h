#include <core/image.h>
#include <string>
#include <vector>

namespace naivebayes {

using std::vector;
using std::string;

/**
 * Stores all the data for all the images, including all the training dataset variables
 */
class Model {
 public:
  //The Laplacian constant for training the model
  const size_t kLaplaceSmoothingFactor = 1;

  /**
   * Default constructor of the Model class
   */
  Model();

  /**
   * Overloaded constructor of the Model class
   * @param frequency_map_size The desired size of the frequency_map_ variable
   */
  Model(size_t frequency_map_size);

  Model(std::map<size_t, vector<vector<double>>> map);

  /**
   * Overloads the >> operator for a given image
   * @param istream The input stream given
   * @param image The instance of this Image class
   * @return The input stream after operator overload
   */
  friend std::istream &operator >>(std::istream &istream, Model& model);

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
  void AddNumOfImagesInClass();

  vector<size_t> GetNumOfImagesInClass() const;

  vector<double> GetPriorProbabilities() const;
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
   * Sets value of image_list_ variable to variable passed in
   * @param image_list The vector of Image elements in a file
   */
  void SetImageList(const vector<naivebayes::Image> &image_list);

  /**
   * Gets the list of Image elements in a file
   * @return Vector of Image elements in a file
   */
  vector<naivebayes::Image> GetImageList() const;

  /**
   * Reads labels from file given a filename
   * @param filename The file name of the training labels file
   */
  void ReadLabels(const string &filename);

  size_t ClassifyImage(naivebayes::Image &image);

  double FindProbabilityOfShadingAtPoint(size_t image_class, std::pair<size_t, size_t> pair);

  vector<double> CalculatePriorProbabilities();

  vector<size_t> GetBestClassList() const;

  std::map<size_t, vector<vector<double>>> GetFeatureProbMap() const;
 private:
  /**
   * Updates frequency_map_ variable with new pixels for a certain class
   * @param s The string representing the current line of the text file
   * @param image_class The class of the current Raster image
   */
  void UpdateFrequencyMap(const string& s, size_t image_class);

  void UpdateFeatureProbMap(size_t image_class);

  /**
   * Adds current image to image_list_ variable
   * @param char_vec The current vector of characters in the current line of the text file
   * @param current_image The current image
   * @param image_class The class of the current image
   */
  void AddCurrentImageToList(const vector<char> &char_vec, naivebayes::Image &current_image, size_t image_class);

  /**
   * Initializes frequency_map_ variable to all zeros
   * @param frequency_map_size The desired size of the frequency map
   */
  void InitializeFrequencyMap(size_t frequency_map_size);

  void InitializeFeatureProbMap(size_t feature_prob_map_size);

  //vector that stores all the training labels
  vector<int> training_label_vec_;

  //2d vector that stores all images in training dataset
  vector<vector<char>> training_image_vec_;

  //vector of all Images
  vector<naivebayes::Image> image_list_;

  //map that stores a 2d-vector of how many times a certain pixel is shaded corresponding to each class of image
  map<size_t, vector<vector<size_t>>> frequency_map_;

  size_t image_vector_key_; //tracks current key of image corresponding to training label

  size_t curr_row_; //tracks current row of the image to be scanned

  vector<double> prior_probabilities_;

  vector<size_t> num_in_class_;

  std::map<size_t, vector<vector<double>>> feature_prob_map_;

  vector<size_t> best_class_list;

  size_t image_dim_;

  bool prob_map_set_;

};
}  // namespace naivebayes
