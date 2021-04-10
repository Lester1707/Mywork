from myKNN import KNN
import numpy as np
import pandas as pd
from sklearn.datasets import load_breast_cancer
from sklearn.metrics import confusion_matrix
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import train_test_split

breast_cancer = load_breast_cancer()
X = pd.DataFrame(breast_cancer.data, columns=breast_cancer.feature_names)
X = X[['mean area', 'mean compactness']]
y = pd.Categorical.from_codes(breast_cancer.target, breast_cancer.target_names)
y = pd.get_dummies(y, drop_first=True)

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=1)

knn1 = KNeighborsClassifier(n_neighbors=5, metric='euclidean')
knn1.fit(X_train, y_train.values.ravel())
y_pred = knn1.predict(X_test)

y_test = y_test.values.ravel()

X_tmp = [[x1] for x1 in X_train['mean area']]
i = 0
for element in X_train['mean compactness']:
	X_tmp[i].append(element)
	i += 1
X_train = X_tmp

X_tmp = [[x1] for x1 in X_test['mean area']]
i = 0
for element in X_test['mean compactness']:
	X_tmp[i].append(element)
	i += 1
X_test = X_tmp

count = 0
knn2 = KNN(X_train, y_train.values.ravel())
for j in range(len(X_test)):
	if y_test[j] == knn2.classification(5, X_test[j]):
		count += 1
print(float(count)/len(y_pred))

count = 0
for j in range(len(y_pred)):
	if y_pred[j] == y_test[j]:
		count += 1
print(float(count)/len(y_pred))
