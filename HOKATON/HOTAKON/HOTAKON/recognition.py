# TODO:
# 1. Распознаём точки, cгруппой

class Coord:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    pass

class Element:
    def __init__(self, coord, name, elements = []):
        self.coord    = coord
        self.name     = name
        self.elements = elements

    def append(self, el, connection_type):
        self.elements.append([el,connectioin_type])

def recognize(sticks,letters):
    normilize_points(sticks,letters)
    start_el = generate_list(sticks,letters)
    return start_el


