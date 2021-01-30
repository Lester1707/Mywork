def findPoint(points, coord):
    for p in points:
        if p.coord.x == coord[0] and p.coord.y == coord[1]:
            return p

def generate(connections, points):
    print("---> (generate) input points")
    print(len(points))
    print("\n".join([str(p) for p in points]))
    print("---> (generate) connections")
    print("\n".join(str(con) for con in connections))
    for p in points:
        diff = 0
        for i in range(len(connections)):
            line = connections[i-diff]
            if line[0][0] == p.coord.x and line[0][1] == p.coord.y:
                addEl = findPoint(points, line[1])
                del connections[i-diff]
                diff += 1
            elif line[1][0] == p.coord.x and line[1][1] == p.coord.y:
                addEl = findPoint(points, line[1])
                del connections[i-diff]
                diff += 1
            else:
                continue
            add = 1
            for Dup in p.elements:
                if Dup[0].coord == addEl.coord:
                    Dup[1] += 1
                    add = 0
                    break
            if add == 1:
                p.append(addEl, 1)

    print("---> Out from generate_list")
    # for p in points:
    #     print(p)
    print("\n".join([str(p) for p in points]))

    return points[0]

