# Decision Tree #
Decision Trees are an approach to solving classification problems on discrete valued attributes.

Included in this repository is the dataset from [UCI Machine Learning Repository on Car Evaluation](http://archive.ics.uci.edu/ml/datasets/Car+Evaluation)
The data is split into training and testing data in the files `car_data_train.csv` and `car_data_test.csv` respectively, and the full dataset is in `car_data.csv`.

The simplest way to compile is using [scons](http://www.scons.org/). In the directory, run the command `scons`.

## Usage ##

To build and test a decision tree, run the following command:

`./DecisionTrees --train car_data_train.csv --test car_data_test.csv`

To build a decision tree and classify unlabeled data, run the following command:

`./DecisionTrees --train car_data_train.csv --classify car_data_unlabeled.csv car_data_labeled.csv`

### Notes ###

In Windows PowerShell it seemed that to specify a space, regular double (or single) quotes don't work. Namely, it seems that when I run:

`.\DecisionTrees.exe -tr .\car_data_train.csv -te .\car_data_test.csv -sp " "`

It doesn't register the " " as an argument. The only solution I've found thus far is to specify with triple quotes, like so:

`.\DecisionTrees.exe -tr .\car_data_train.csv -te .\car_data_test.csv -sp """ """`
