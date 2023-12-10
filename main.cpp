#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <cmath>
#include "csvstream.hpp"

using namespace std;



class Classifier {
    private:
        bool debug;
        int total_posts; //number of posts in file
        int total_test_posts;
        set<string> words; //set of all unique words 
        set<string> test_words;
        map<string, int> word_post_freq; //number of posts containing a specific word;
        map<string, int> label_count; //number of training posts with each label 
        map<string, int> test_label_count; //number of training posts with each label 
        map<pair<string, string>, int> label_word_freq; 
        map<pair<string, string>, int> test_label_word_freq;

        
      // EFFECTS: Return a set of unique whitespace delimited word. 
      set<string> unique_words(const string &str) {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
          words.insert(word);
        }
        return words;
      }
     

      //inserts all unique words in a word file to words
      //and fruquencies of every word occurance in posts
      void unique_words_set_maker(string word_file) {
        set<string> unique_words_in_file = unique_words(word_file);
        for(auto &word : unique_words_in_file) {
          words.insert(word);
          if(unique_words_in_file.find(word) != unique_words_in_file.end()) {
              word_post_freq[word]++;
          }
        }
      }

      /**

      //returns size of unique words
      int vocab_size() {
        return words.size();
      }
      */

      void label_post_count(string label) {
        label_count[label]+=1;
      }

      void test_label_post_count(string label) {
        test_label_count[label]+=1;
      }

      void label_word_post_count(const string post, const string label) {
        set<string> unique_words_in_post = unique_words(post);
        for(auto &word: unique_words_in_post) {
            label_word_freq[{word, label}]++;
        }
      }

      void test_label_word_post_count(const string post, const string label) {
        set<string> unique_words_in_post = unique_words(post);
        for(auto &word: unique_words_in_post) {
            test_label_word_freq[{word, label}]++;
        }
      }


 public:
      Classifier(bool debuggie) :  debug(debuggie), total_posts(0), total_test_posts(0) {}

      void train_func(string word_file) {
        csvstream csvin(word_file);
        map<string, string> line;
        if(debug) {
            cout << "training data:" << endl;
        }
        while(csvin >> line) {
            total_posts++;
            unique_words_set_maker(line["content"]);
            label_post_count(line["tag"]);
            label_word_post_count(line["content"], line["tag"]);
            if(debug) {
                cout << "  label = " << line["tag"] << ", content = " << line["content"] 
                << endl;
            }
        }
        cout << "trained on " << total_posts << " examples" << endl;
        
        if (debug) {
          cout << "vocabulary size = " << words.size() << endl;
          cout << endl;
          debug_classes();
          debug_parameters();
          cout << endl;
        } else {
          cout << endl << endl;
        }
      }

      void debug_classes() {
        cout << "classes:" << endl;
        for(auto &posts: label_count) {
            string label = posts.first;
            int num_posts = label_count.at(label);
            cout << "  " << label << ", " << num_posts << " examples, log-prior = " 
            << log(1.0 * (static_cast<double>(num_posts)/total_posts)) << endl;
        }
      }

      void debug_parameters() {
        cout << "classifier parameters:" << endl;
         for(auto &posts: label_count) {
          string label = posts.first;
          for(auto &word: words) {
                if(label_word_freq.count({word, label})) {
                    int example_count = label_word_freq.at({word, label});
                    cout << "  " << label << ":" << word << ", count = " << example_count
                    << ", log-likelihood = " 
                    << log(1.0*example_count/label_count.at(label)) << endl;
                }
            }
         }

      }

      

      pair<string, double> label_prob_calc(const string word_file) {
        cout.precision(3);
        
        //all the unique words in test file
        set<string> posts_unique_words = unique_words(word_file);
        string highest_label;
        
        double highest_log_prob = -2147483648; 
        
        
        //compare those words to training file info
        for(auto &iter: label_count) {
            double curr_prob = 0;
            string curr_label = iter.first;
            curr_prob += log(1.0*label_count[curr_label]/total_posts);  //log prior
            for(auto &word: posts_unique_words) {
              //word does not show up in training data
                if(word_post_freq.find(word) == word_post_freq.end()) {
                    curr_prob += log(1.0/total_posts);
                }
                //word shows up in training data but not in label
                else if(label_word_freq.find({word, curr_label}) 
                        == label_word_freq.end()) {
                    curr_prob += log(1.0*word_post_freq[word]/total_posts);
                }
                //word shows up in training data + in label
                else {
                    curr_prob += log(1.0*label_word_freq[{word, curr_label}]
                                /label_count[curr_label]);
                }
            }
            if(curr_prob > highest_log_prob) {
                highest_log_prob = curr_prob;
                highest_label = curr_label;
            }
            //
            if(curr_prob == highest_log_prob) {
              if(curr_label < highest_label) {
                        highest_label = curr_label;
                    }
            }
        }
 
        return {highest_label, highest_log_prob};
        
      }
      

      void test(string test_file) {
        cout << "test data:" << endl;

        csvstream test_in(test_file);
        map<string, string> test;
        int correct = 0;

        while (test_in >> test) {
          total_test_posts++;

          string predict = label_prob_calc(test["content"]).first;
          double score = label_prob_calc(test["content"]).second;

          if (test["tag"] == predict) {
            correct++;
          }

          cout << "  correct = " << test["tag"] << ", predicted = " 
               << predict << ", log-probability score = "
               << score << endl;
          cout << "  content = " << test["content"] << endl << endl;
        }

        cout << "performance: " << correct << " / " << total_test_posts 
             << " posts predicted correctly" << endl;

      }
};

int main(int argc, char** argv) {
  
  //added bc comparing to string literal caused problems 
  string debug = "--debug";

  //for log probability calculations 
  cout.precision(3);

  //map to store training + testing data 
  string trainer_file = argv[1];
  string test_file = argv[2];
  
  //making sure # of arguments/content is correct
  if (argc < 3 || argc > 4) {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return -1;
  } else if (argc == 4 && argv[3] != debug) {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return -1;
  } else {

    //opening files and checking it works properly
    try {
      csvstream test_file(argv[2]);
    }
    catch (csvstream_exception &e) {
      cout << "Error opening file: " << trainer_file << endl;
    }
     try{
        csvstream csvin(test_file);
    }
    catch(const csvstream_exception &e) {
        cout << "Error opening file: " << test_file << endl;
    }



    //if there's a --debug command
    bool debug = false;
    if(argc == 4) { debug = true; } 
    
    //call helper functions to analyse posts map 
    
    Classifier c(debug);
    c.train_func(trainer_file);
    c.test(test_file);
    
  }
}