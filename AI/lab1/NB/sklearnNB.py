import numpy as np
from sklearn.naive_bayes import CategoricalNB
import string
from tensorflow.keras.preprocessing.text import Tokenizer
from tensorflow.keras.preprocessing.sequence import pad_sequences

x_train = []
y_train = []
with open("train.txt", "r", encoding="utf-8") as file:
	colom = 0
	while True:
		n_str = file.readline()
		if n_str == '':
			break
		colom += 1
		n = int(n_str)
		words = []
		for i in range(n):
			colom += 1
			text = file.readline()
			if text == '':
				continue
			for word in text.split(" "):
				word = word.lower()
				word = word.rstrip('\n')
				word = word.rstrip(string.punctuation)
				if word == '':
					continue
				if word == '\t':
					continue
				words.append(word)
		x_train.append(words)
		text = file.readline()
		colom += 1
		for tag in text.split(","):
			y_train.append(tag.rstrip('\n'))
			break

tok = Tokenizer(num_words = 5000)
tok.fit_on_texts(x_train)
seq = tok.texts_to_sequences(x_train)
x_train = pad_sequences(seq, maxlen = 50)

clf = CategoricalNB()
clf.fit(x_train, y_train)

y_ans = clf.predict(x_train[5000:5500])
count = 0
for i in range(len(y_ans)):
	if y_ans[i] == y_train[5000:5500][i]:
		count += 1
print(float(count)/len(y_ans))
