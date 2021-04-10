
class KNN(object):
	def __init__(self, data_in, type_in, weights_in = {}):
		dim = len(data_in[0])
		if len(type_in) != len(data_in):
			raise ValueError("bad data")
		for element in data_in:
			if dim != len(element):
				raise ValueError("bad dimension")
		if len(weights_in) == 0:
			self.weights = {x : 1 for x in range(dim)}
		else:
			if len(weights_in) != dim:
				raise ValueError("bad weights")
			self.weights = weights_in
		self.data = [[float(x) for x in y] for y in data_in]
		self.type = type_in

	def metrics(self, place1, place2):
		if len(place1) != len(place2):
			raise ValueError("bad size")
		r = 0
		for i in range(len(place1)):
			r += self.weights[i]*(place1[i] - place2[i])**2
		return r**0.5

	def classification(self, k, object_place_in):
		object_place = [float(x) for x in object_place_in]
		if len(object_place) != len(self.data[0]):
			raise ValueError("bad data")
		dists = {}
		for i in range(len(self.data)):
			dists[self.metrics(object_place, self.data[i])] = i
		types = set(self.type)
		Q = {type : 0 for type in types}
		count = 0
		for key, val in dists.items():
			if count == k:
				break
			if key == 0:
				Q[self.type[val]] = float("inf")
			else:
				Q[self.type[val]] += key**(-2)
			count += 1
		max = 0
		for key, val in Q.items():
			if val > max:
				max = val
				best_key = key
		return best_key

