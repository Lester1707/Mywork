import math

class Coord:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    # попадает ли переданная точка в переданный радиус
    def near(self, other, rad):
        diff = (self - other).dist()
        if diff < rad:
            return True
        return False

    # TODO: make a property
    def dist(self):
        return math.sqrt(self.x**2 + self.y**2)

    # ==
    def __eq__(self, other):
        if self.x == other.x and self.y == other.y: return True
        return False
    # -
    def __sub__(self, other):
        return Coord(self.x-other.x,self.y-other.y)
    # +
    def __add__(self, other):
        return Coord(self.x+other.x,self.y+other.y)
    # +=
    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        return self
    # /
    def __div__(self, other):
        return Coord(self.x/other, self.y/other)
    # /=
    def __idiv__(self, other):
        self.x /= other
        self.y /= other
        return self
    def __rdiv__(self, other):
        self.x /= other
        self.y /= other
        return self
    # print
    def __str__(self):
        return("({},{})".format(self.x,self.y))
    # print from array
    def __repr__(self):
        return self.__str__()

class Element:
    def __init__(self, coord, name, elements = []):
        self.coord    = coord
        self.name     = name
        self.elements = elements

    def append(self, el, connection_type):
        self.elements.append([el,connection_type])

    # print
    def __str__(self):
        out = "{}:{}:".format(self.name, self.coord)
        out += "other({})".format(
                "\n".join(["{}::{}".format(el[0].coord, el[-1]) for                  el in self.elements]))
        return(out)
    # print from array
    def __repr__(self):
        return self.__str__()
