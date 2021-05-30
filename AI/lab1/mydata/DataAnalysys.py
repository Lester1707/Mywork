import csv
from tensorflow.keras.preprocessing.text import Tokenizer
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras import utils
from sklearn.neighbors import KNeighborsClassifier
import pandas
import nltk
from nltk.stem import PorterStemmer
from nltk.stem import WordNetLemmatizer
from nltk.probability import FreqDist
import re
import string
from LabML_1 import KNN

nltk.download('punkt')

max_review = 1000
max_words = 30

nltk.download('wordnet')
stopwords=['this','that','and','a','we','it','to','is','of','up', 'are', 'as','the']


def ampliment(texts, start, end):
	freq = {}
	ps = PorterStemmer()
	lem = WordNetLemmatizer()
	for i in range(start, end):
		step = 0
		texts[i] = texts[i].strip()
		texts[i] = texts[i].lower()
		tmp = texts[i].split(' ')
		for word in tmp:
			start_word = word
			word = ps.stem(word)
			word = re.sub(r'\d+', '', word)
			if word in stopwords:
				word = 'W'
			if word == start_word:
				continue
			if step == 0 or step + 1 == len(tmp):
				texts[i] = texts[i].replace(start_word + ' ', word + ' ')
			else:
				texts[i] = texts[i].replace(' ' + start_word + ' ', ' ' + word + ' ')
			step += 1
	an_text = ""
	for text in texts:
		text = text.replace('/', "")
		text = text.replace('<', "")
		text = text.replace('>', "")
		text = text.replace('-', " ")
		text = text.replace(',', "")
		text = text.replace('.', "")
		text = text.replace('?', "")
		text = text.replace('!', "")
		for word in text.split(' '):
			freq[word] = freq.get(word, 0) + 1
		an_text += text + " "
	b_tok = nltk.word_tokenize(an_text)
	print("count of bigramms: ", len(list(nltk.bigrams(b_tok))))
	sorted_val = sorted(freq.values(), reverse=True)
	sort_freq = {}
	for i in sorted_val:
		for k in freq.keys():
			if freq[k] == i:
				sort_freq[k] = freq[k]
				break
	print("freq (W = count of stop word):")
	print(sort_freq)
	tok = Tokenizer(num_words = 5000)
	tok.fit_on_texts(texts)
	#print(tok.word_index)
	seq = tok.texts_to_sequences(texts)
	return pad_sequences(seq, maxlen = max_words)

data = pandas.read_csv('base.csv')
x_train = data['review'][:max_review]
print(x_train)
#x_answer = data['review'][max_review:2*max_review]
#print(x_answer)

x_train = ampliment(x_train, 0, max_review)
print(x_train)
#x_answer = ampliment(x_answer, max_review, 2*max_review)
#print(x_answer)


y_train = data['sentiment'][:max_review]
print(y_train)
#y_answer = data['sentiment'][max_review:2*max_review]
#print(y_answer)

