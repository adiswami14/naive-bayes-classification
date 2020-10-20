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
  const size_t kLaplaceSmoothingFactor = 1; //The Laplacian constant for training the model

  /**
   * Default constructor of the Model class
   */
  Model();

  /**
   * Overloaded constructor of the Model class
   * @param image_dim The dimensions of each Image
   */
  Model(size_t image_dim);

  /**
   * Overloaded constructor used for testing models
   * @param map The map of feature probabilities
   */
  Model(std::map<size_t, vector<vector<double>>> map);

  /**
   * Overloaded constructor for testing models
   * @param image_dim The dimensions of each Image that is read in
   * @param map The map of feature probabilities
   */
  Model(size_t image_dim, std::map<size_t, vector<vector<double>>> map);

  /**
   * Overloads the >> operator for a given image
   * @param istream The input stream given
   * @param Model The instance of this Model class
   * @return The input stream after operator overload
   */
  friend std::istream &operator >>(std::istream &istream, Model& model);

  /**
   * Sets value of training_label_vec_ variable to variable passed in
   * @param training_label_vec The vector of ints corresponding to all the training label numbers
   */
  void SetTrainingLabelVec(const vector<int> &training_label_vec);

  /**
   * Returns the training_label_vec_ variable
   * @return Private variable training_label_vec_
   */
  vector<int> GetTrainingLabelVec() const;

  /**
   * Adds number of images in each class to num_in_class_ variable
   */
  void AddNumOfImagesInClass();

  /**
   * Gets the num_in_class_ variable
   * @return A vector tracking how many images of each class are in a dataset
   */
  vector<size_t> GetNumOfImagesInClass() const;

  /**
   * Gets the prior_probabilities_ variable
   * @return A vector of all prior probabilities
   */
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
   *
   * @return
   */
  vector<size_t> GetBestClassList() const;

  /**
   *
   * @return
   */
  std::map<size_t, vector<vector<double>>> GetFeatureProbMap() const;

  /**
   * Reads labels from file given a filename
   * @param filename The file name of the training labels file
   */
  void ReadLabels(const string &filename);

  /**
   * Classifies a given image and returns its best fit
   * @param image The instance of Image to classify
   * @return The class that best fits the image
   */
  size_t ClassifyImage(const Image &image);

  /**
   * Finds probability of finding shading for a class of image at a specific point
   * @param image_class The class of the image
   * @param pair The position of the image
   * @return The feature probability at a certain point for a certain class
   */
  double FindProbabilityOfShadingAtPoint(size_t image_class, std::pair<size_t, size_t> pair);

  /**
   * Calculates prior probabilities of dataset
   * @return A vector of prior probabilities for all classes
   */
  vector<double> CalculatePriorProbabilities();
 private:
  /**
   * Updates frequency_map_ variable with new pixels for a certain class
   * @param s The string representing the current line of the text file
   * @param image_class The class of the current image
   */
  void UpdateFrequencyMap(const string& s, size_t image_class);

  /**
   * Updates feature_prob_map_ variable with each new class
   * @param image_class The class of current image
   */
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

  /**
   * Initializes feature_prob_map_ variable to all zeros
   * @param feature_prob_map_size The desired size of the feature probability map
   */
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

  vector<double> prior_probabilities_; //vector of prior probabilities

  vector<size_t> num_in_class_; //vector tracking number of images belonging to certain class in dataset

  std::map<size_t, vector<vector<double>>> feature_prob_map_; //map of all feature probabilities

  vector<size_t> best_class_list; //vector of the classifications of all images in order

  size_t image_dim_; //The given dimensions of each image passed in

  bool prob_map_set_; //Keeps track of whether a feature probability map is passed in via constructor
};
}  // namespace naivebayes
