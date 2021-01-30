import copy as co

def list_in_list(a, b):
    if len(a) != len(b):
        return 0
    k = 0
    c = 0
    for i in range(len(a)):
        for j in range(len(b)):
            if a[i] == b[j]:
                k += 1
                c = 1
                break
        if c == 1:
            c = 0
            continue
    if k == len(a):
        return 1
    else:
        return 0

def my_func(mat, deep, use, bad, good, all):
    if len(use) == 0:
        out.append(good)
        return 0
    use_h = co.deepcopy(use)
    for k in range(len(use_h)):
        goods.append(co.deepcopy(good))
        uses.append(co.deepcopy(use))
        bads.append(co.deepcopy(bad))
        if mat[all.index(use[k])][all.index(use[k])] == 1:
            continue
        good.append(use[k])
        use.pop(k)
        for i in range(len(use)):
            if mat[all.index(good[len(good) - 1])][all.index(use[i])] == 1:
                bad.append(all.index(use[i]))
        for i in range(len(use)):
            if mat[all.index(use[i])][all.index(good[len(good) - 1])] == 1:
                if all.index(use[i]) not in bad:
                    bad.append(all.index(use[i]))
        for j in range(len(bad)):
             if bad[j] in use:
                use.pop(use.index(bad[j]))
        my_func(mat, deep + 1, use, bad, good, all)
        use = co.deepcopy(uses[deep])
        bad = co.deepcopy(bads[deep])
        good = co.deepcopy(goods[deep])
        use_h = co.deepcopy(use)
        bads.pop(deep)
        uses.pop(deep)
        goods.pop(deep)

out = []
matrix = []
bad_nodes = []
good_nodes = []
m_h = []
goods = []
uses = []
bads = []

print("enter the size matrix:")
size = int(input())
for i in range(size):
    for j in range(size):
        print("enter", i + 1, "line and", j + 1, "column:")
        m_h.append(int(input()))
    matrix.append(m_h)
    m_h = []
print("your graph:")

matrix = [[0, 1, 0, 0, 1, 1, 0, 0], [1, 0, 1, 1, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 1, 0, 0, 0], [0, 0, 0, 1, 0, 1, 0, 0], [0, 0, 0, 0, 1, 0, 1, 0], [0, 0, 0, 0, 0, 0, 0, 1], [0, 0, 0, 1, 0, 1, 0, 0]]
size = 8

for i in range(size):
    print(matrix[i])

all_nodes = list(range(size))
use_nodes = co.deepcopy(all_nodes)
my_func(matrix, 0, use_nodes, bad_nodes, good_nodes, all_nodes)

if len(out) != 0:
    i_h = 0
    j_h = 0
    while i_h < (len(out)):
        while j_h < (len(out)):
            if i_h == j_h:
                j_h += 1
                continue
            if list_in_list(out[i_h], out[j_h]):
                out.pop(j_h)
                continue
            j_h += 1
        i_h += 1
        j_h = 0

print("your answer:")

if len(out) != 0:
    for i in range(len(out)):
        for j in range(len(out[i])):
            out[i][j] += 1
    for i in range(len(out)):
        print(out[i])
else:
    print(out)

    
    