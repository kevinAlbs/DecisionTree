# Decision Tree #
Decision Trees are an approach to solving classification problems on discrete valued attributes.

The simplest way to compile is using [scons](http://www.scons.org/). In the project root directory, run the command `scons`.

## Usage ##

### Command Line Interface ###
Included is a minimal command line interface to train, test, and classify unlabeled data.

Using the provided [example dataset](http://archive.ics.uci.edu/ml/datasets/Car+Evaluation) we can build and test the decision tree using the following command:

`./DecisionTreeCLI --train data/labeled_train.dat --test data/labeled_test.dat`

This gives the output:

```
Testing data from file './data/labeled_test.dat'
Results: 437 of 501 correct
Percentage correct: 87.2255
```

To build a decision tree and classify unlabeled data, run the following command:

`./DecisionTreeCLI --train data/labeled_train.dat --classify data/unlabeled.dat data/labeled.dat`

Results in creating a new file named `data/labeled.dat` which contains the additional lablel attribute with the predicted value.

### DecisionTree class ###

A small example on using the DecisionTree class to train and test data from files:

```cpp
#include <iostream>
#include "DecisionTree.h"
using namespace std;

int main(){
  DecisionTree tree(",", false, "label_attribute");
  tree.train("trainfile.csv");
  pair<int,int> result = tree.testLabeled("testfile.csv");
  cout << "Got " << result.first << " out of " << result.second << " correct\n";
}
```

### Example Dataset ###
Included in this repository is the dataset from [UCI Machine Learning Repository on Car Evaluation](http://archive.ics.uci.edu/ml/datasets/Car+Evaluation) in the directory `data`.

The data is split into training and testing data in the files `car_data_train.dat` and `car_data_test.dat` respectively, and the full dataset is in `car_data.dat`.

The data is space separated (although you can use any string separator). The labeled data (training and testing) includes the label "acceptable":
```
buying maint doors persons lug_boot safety acceptable
vhigh vhigh 2 2 med high unacc
vhigh vhigh 2 4 small high unacc
vhigh vhigh 2 4 med high unacc
[...]
```

The unlabeled data does not:
```
buying maint doors persons lug_boot safety
vhigh med 3 2 big high
vhigh med 3 4 med med
vhigh med 3 4 med high
```
### Upcoming Features ###

- Methods to instantiate tree with a vector of string vectors (as opposed to just a file path)
- During classification if the decision tree has no path to take, it returns the empty string. Instead, this will change to return the most probably classification based on weighting all possible other values for the missing attribute value and returning the most probable along each path.

### Notes ###

In Windows PowerShell it seemed that to specify a space, regular double (or single) quotes don't work. Namely, it seems that when I run:

`.\DecisionTrees.exe -tr .\car_data_train.dat -te .\car_data_test.dat -sp " "`

It doesn't register the " " as an argument. The only solution I've found thus far is to specify with triple quotes, like so:

`.\DecisionTrees.exe -tr .\car_data_train.dat -te .\car_data_test.dat -sp """ """`
