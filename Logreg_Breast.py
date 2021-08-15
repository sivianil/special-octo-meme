# -*- coding: utf-8 -*-
"""
Created on Sun Aug 15 09:24:11 2021

@author: BODDAPATI ANILKUMAR
"""

#Logistic regression practical case study on breast cancer dataset

#Import libraries
import pandas as pd

#Importing the dataset
dataset = pd.read_csv('breast_cancer.csv')
X = dataset.iloc[:, 1:-1].values
y = dataset.iloc[:, -1].values

#Splitting dataset into the Training set and Test set
from sklearn.model_selection import train_test_split
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state = 0)

#Training the Logistic Regression model on the TRaining set
from sklearn.linear_model import LogisticRegression
classifier = LogisticRegression(random_state=0)
classifier.fit(X_train, y_train)

#Predicting Test set results
y_pred = classifier.predict(X_test)

#Making confusion matrix
from sklearn.metrics import confusion_matrix
cm = confusion_matrix(y_test, y_pred)
print(cm)

#Computing the accuracy with k-Fold Cross Validation
from sklearn.model_selection import cross_val_score
accuracies  = cross_val_score(estimator = classifier, X = X_train, y = y_train, cv = 10)
print("Accuracy: {:.2f} %".format(accuracies.mean()*100))
print("Standard Deviation: {:.2f} %".format(accuracies.std()*100))