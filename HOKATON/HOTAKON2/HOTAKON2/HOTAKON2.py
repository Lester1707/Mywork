#!python3

import recognition as rg
import drawing
import toSmiles
# JUST FOR TEST
sticks = [
        [[4,6],[4,7.2]],
        [[4,7.2],[5.2,8.6]],
        [[5.2,8.6],[4.1,9.7]],
        [[5.2,8.6],[7.5,8.7]],
        [[7.5,8.7],[8.3,7.5]],
        [[8.3,7.5],[8.3,5.7]],
        [[4,6],[5,4.5]],
        [[5,4.5],[7.2,4.5]],
        [[7.2,4.5],[8.3,5.7]],
        # valent lines
        [[4.5,7.3],[5.25,8.2]],
        [[4.5,6.05],[5.1,5.1]],
        [[7.2,5.2],[7.9,6]],
        [[7.25,8.25],[7.75,7.5]],
        # rest lines
        [[8.3,5.9],[9.9,5]],
        [[10.9,5],[12.1,6]],
        [[12.1,6],[12,7.2]],
        [[12.1,6],[13.1,5.5]],
        [[13.1,5],[13.1,3.8]],
        [[13.55,5],[13.55,3.8]],
        [[13.55,5.5],[14.5,6.7]]
        ]
letters = [
        [[3,9.7],[4.2,11.2]],
        [[9.8,4.3],[10.9,5.2]],
        [[10.55,7],[13.6,7.4]],
        [[13,2.5],[14,3.5]]
        ]

cord1 = rg.Coord(40, 500)
cord2 = rg.Coord(40+100, 500-173)
cord3 = rg.Coord(40+100+200, 500-173)
cord4 = rg.Coord(40+200+200, 500)
cord5 = rg.Coord(40+100+200, 500+173)
cord6 = rg.Coord(40+100, 500+173)
start_el = rg.Element(cord1, "Р", [[rg.Element(cord2, 'HO', [[rg.Element(cord5, 'HO', []), 1], [rg.Element(cord6, 'HP', []), 2]]), 3], [rg.Element(cord5, 'HO', []), 2], [rg.Element(cord3, 'CN', [[rg.Element(cord5, 'HO', []), 2]]), 2], [rg.Element(cord4, 'NH', [[rg.Element(cord5, 'HO', []), 2], [rg.Element(cord3, 'CN', []), 2]]), 2]])


#start_el = recognition.recognize(sticks,letters,0)
smiles_code = toSmiles.generate(start_el)
print(smiles_code)
#scheme_file_name = drawing.build_image(start_el)
