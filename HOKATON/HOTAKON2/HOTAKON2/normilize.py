from copy import deepcopy
from utils import Coord, Element

IMG_SIZE = 800
ACCURACY = 750
def normilize_points(sticks,letters,img_size):
    visited     = [0 for _ in range(len(sticks)*2)]
    visited_let = [0 for _ in range(len(letters) * 2)]
    points = []
    # JUST FOR TESTING
    img_size = IMG_SIZE
    delta    = img_size / ACCURACY
    # FOR TESTING
    delta    = 1.1
    # print("Calculated delta is {}".format(delta))
    for i, first_vec in enumerate(sticks):
        for ip, pt in enumerate(first_vec):
            print(",".join([str(i) for i in visited]))
            if visited[i*2 + ip]: continue
            visited[i*2 + ip] |= 1
            name = "C"
            first_coord = Coord(*pt)
            print("---> {}: parsing coord {}".format(i,first_coord))
            # находим вектора с достаточно близкими точками
            # для каждого вектора кроме себя
            coord_qty = 1
            coord_sum = deepcopy(first_coord)
            # Ссылка на все точки, который надо нормализировать
            pts = [pt]
            for j, second_vec in enumerate(sticks[i+1:],start=i+1):
                print("parsing vec {}".format(j))
                for jp,comp_pt in enumerate(second_vec):
                    if visited[j*2+jp]: continue
                    second_coord = Coord(*comp_pt)
                    if second_coord.near(first_coord,delta):
                        visited[j*2+jp] |= 1
                        print("coord {} is close to current"\
                            .format(second_coord))
                        print("diff is {}, delta is {}".\
                            format((first_coord - second_coord).dist(), delta))
                        coord_qty += 1
                        coord_sum += second_coord
                        pts.append(comp_pt)
                    else:
                        print("coord {} isn't close to current"\
                            .format(second_coord))
                        print("diff is {}, delta is {}".\
                            format((first_coord - second_coord).dist(), delta))
            # После того как все просумировалось
            # Проверяем, стоит ли рядом какая то буква
            coord_sum.__idiv__(coord_qty)
            for letter in letters:
                # Находим координату центра вектора
                center = \
                (Coord(*letter[0])+Coord(*letter[-1])).__idiv__(2)
 
                if (center.near(coord_sum,delta)):
                    # TODO: uncomment when implement
                    # letter = recognize_letter(letter)
                    print("letter is close")
                    print("assigning coord sum to letter center")
                    print("coord {} isn't close to current"\
                        .format(second_coord))
                    print("diff is {}, delta is {}".\
                        format((first_coord - second_coord).dist(), delta))
                    name = "I"
                    coord_sum = center
            # Проходимся по каждой точке и исправляем её коор-ты
            for point in pts:
                point[0] = coord_sum.x
                point[1] = coord_sum.y

            el = Element(coord_sum, name)
            # print("created element {}".format(el))
            points.append(el)
    return points

def recognize_letter(letter_coord):
    pass
