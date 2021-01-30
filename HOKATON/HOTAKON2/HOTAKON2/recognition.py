# TODO:
# 1. Распознаём точки, cгруппой

import generate_list
import normilize

from utils import Coord, Element

def recognize(sticks,letters,img_size):
    points = normilize.normilize_points(sticks,letters,img_size)
    return generate_list.generate(sticks,points)

