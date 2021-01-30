#!python3

import sys
import numpy as np

# алгоритм для разбора изображения на буквы и палки
import vectorize    
# разбор массива на матрицу смежности
import recognition
# Отрисовка финальной версии
import drawing

if __name__ == "__main__":
    assert len(sys.argv) > 1, "pass in filename: {}".format(sys.argv)
    print(sys.argv)
    img_filename = sys.argv[-1]
    print("\tcall vectorize with {} filename".format(img_filename))
    sticks, letters = vectorize.vectorize(img_filename) 
    assert inp, "vvvvvv\nvectorize.vectorize(img_filename) is empty"
    print("\tout from vectorize")
    print(inp)
    print("\tcall to vector recognition")
    start_el = recognition.recognize(sticks, letters)
    scheme_file_name = drawing.build_image(start_el)
    
