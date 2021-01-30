import random
import matplotlib.pyplot as plt
import numpy as np
import copy

def sum_power(a, b):
     p_help = 0
     for i in range(len(a)):
        p_help += b.index(a[i]) + 1
     return p_help/(2*len(b)*(len(b) + 1))
 
print("choouse event: \n 1) detailed analysis of one game\n 2) game set analysis\n")
inclav = int(input("enter the event\n"))
lencol = int(input("what deck count you want? 36 or 52?\n"))
if lencol == 52:
    carts = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
else:
    carts = ['6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
inclav1 = int(input("do you want to enter your carts? If yes print 1; else 0:\n"))
Massivcarts = []
for i in range(len(carts)):
    Massivcarts.append(carts[i])
    Massivcarts.append(carts[i])
    Massivcarts.append(carts[i])
    Massivcarts.append(carts[i])
coloda1 = []
coloda2 = []
if inclav1 == 0:
    random.shuffle(Massivcarts)
    for i in range(len(carts)*2):
        coloda1.append(Massivcarts[i])
    for j in range(len(carts)*2):
        j += len(carts)*2
        coloda2.append(Massivcarts[j])
else:
    i = 0
    while i < 2*len(carts):
        print("enter the card of first deck: ")
        karta = input()
        l = 1
        for j in range(len(carts)):
            if karta == carts[j]:
                l = 0
                c = 0
                for k in range(len(coloda1)):
                    if coloda1[k] == carts[j]:
                        c += 1
                if c > 3:
                    print("wrong format!!!")
                    i -= 1
                    continue
                coloda1.append(carts[j])
        if l == 1:
            print("wrong format!!!")
            continue
        i += 1
    print(coloda1)
    i = 0
    while i < 2*len(carts):
        print("enter the card of second deck: ")
        karta = input()
        l = 1
        for j in range(len(carts)):
            if karta == carts[j]:
                l = 0
                c = 0
                for k in range(len(coloda2)):
                    if coloda2[k] == carts[j]:
                        c += 1
                for k in range(len(coloda1)):
                    if coloda1[k] == carts[j]:
                        c += 1
                if c > 3:
                    print("wrong format!!!")
                    i -= 1
                    continue
                coloda2.append(carts[j])
        if l == 1:
            print("wrong format!!!")
            continue
        i += 1
    print(coloda2)

por = 0
iter = 0
COLODA1_GO = copy.copy(coloda1)
COLODA2_GO = copy.copy(coloda2)
koef = []
power1 = []
power2 = []
WIN1 = 0
WIN2 = 0
iters = []

print("Your play carts:")
print(coloda1)
print(coloda2)
print("________________________________________________________________________________________________________________________")

print("do you want use random?")
ran = int(input("enter 1 if yes, else 0: "))

if inclav == 1:
    n = 1
else:
    n = 1000
    
print("power of first colod: ", sum_power(coloda1, carts))
print("power of second colod: ", sum_power(coloda2, carts))

for k in range(n):
    while True:
        if (len(coloda1) == len(carts)*4 or len(coloda1) == 0):
            break
        iter += 1
        koef.append(len(coloda1)/4/len(carts))
        power1.append(sum_power(coloda1, carts))
        promejut_coloda = []
        karta1 = coloda1.pop(0)
        karta2 = coloda2.pop(0)
        if (carts.index(karta1) == 0 and carts.index(karta2) == len(carts) - 1) or (carts.index(karta1) == len(carts) - 1 and carts.index(karta2) == 0):
            if carts.index(karta1) < carts.index(karta2):
                promejut_coloda.append(karta1)
                promejut_coloda.append(karta2)
                if ran == 1:
                    random.shuffle(promejut_coloda)
                coloda1.append(promejut_coloda[0])
                coloda1.append(promejut_coloda[1])
            else:
                promejut_coloda.append(karta1)
                promejut_coloda.append(karta2)
                if ran == 1:
                    random.shuffle(promejut_coloda)
                coloda2.append(promejut_coloda[0])
                coloda2.append(promejut_coloda[1])
            if inclav == 1:
                print(coloda1)
                print(coloda2)
                print("________________________________________________________________________________________________________________________")   
            continue
        if carts.index(karta1) > carts.index(karta2):
            promejut_coloda.append(karta1)
            promejut_coloda.append(karta2)
            if ran == 1:
                random.shuffle(promejut_coloda)
            coloda1.append(promejut_coloda[0])
            coloda1.append(promejut_coloda[1])
            if inclav == 1:
                print(coloda1)
                print(coloda2)
                print("________________________________________________________________________________________________________________________")
        if carts.index(karta1) < carts.index(karta2):
            promejut_coloda.append(karta1)
            promejut_coloda.append(karta2)
            if ran == 1:
                random.shuffle(promejut_coloda)
            coloda2.append(promejut_coloda[0])
            coloda2.append(promejut_coloda[1])
            if inclav == 1:
                print(coloda1)
                print(coloda2)
                print("________________________________________________________________________________________________________________________")
        if carts.index(karta1) == carts.index(karta2):
            promejut_coloda.append(karta1)
            promejut_coloda.append(karta2)
            if len(coloda1) == 0:
                WIN1 += 1
                por = 1
            if len(coloda2) == 0:
                WIN2 += 1
                por = 1
            if (por == 1):
                break
            promejut_coloda.append(coloda1.pop(0))
            promejut_coloda.append(coloda2.pop(0))
            while carts.index(karta1) == carts.index(karta2):
                if len(coloda1) == 0:
                    por = 1
                    WIN2 += 1
                    break
                if len(coloda2) == 0:
                    WIN1 += 1
                    por = 1
                    break
                karta1 = coloda1.pop(0)
                karta2 = coloda2.pop(0)
                promejut_coloda.append(karta1)
                promejut_coloda.append(karta2)
            if (por == 1):
                break
            if ran == 1:
                random.shuffle(promejut_coloda)
            if (carts.index(karta1) == 0 and carts.index(karta2) == len(carts) - 1) or (carts.index(karta1) == len(carts) - 1 and carts.index(karta2) == 0):
                if carts.index(karta1) < carts.index(karta2):
                    for i in range(len(promejut_coloda)):
                        coloda1.append(promejut_coloda[i])
                else:
                    for i in range(len(promejut_coloda)):
                        coloda2.append(promejut_coloda[i])
                
                continue
            if carts.index(karta1) > carts.index(karta2):
                for i in range(len(promejut_coloda)):
                        coloda1.append(promejut_coloda[i])
            if carts.index(karta1) < carts.index(karta2):
                for i in range(len(promejut_coloda)):
                        coloda2.append(promejut_coloda[i])
            if inclav == 1:
                print(coloda1)
                print(coloda2)
                print("________________________________________________________________________________________________________________________")

    if (por == 1):
       por = 0
       coloda1 = copy.copy(COLODA1_GO)
       coloda2 = copy.copy(COLODA2_GO)
       continue
    if len(coloda1) == 0:
        #print("player1 won!")
        WIN2 += 1
    else:
        #print("player2 won!")
        WIN1 += 1
    coloda1 = copy.copy(COLODA1_GO)
    coloda2 = copy.copy(COLODA2_GO)
    iters.append(iter)
    if inclav != 1:
        iter = 0


if inclav == 1:
    print(iter, "hodov!!!")
else:
	print("median hods:")
	for i in range(len(iters)):
		iter *= i
		iter += iters[i]
		iter /= i + 1
	print(iter)

print("Count wins of fists player:", WIN1)
print("Count wins of second player:", WIN2)

if inclav == 1: 
    plt.plot(np.arange(len(koef)), np.array(koef), label = "carts of first person")
    plt.plot(np.arange(len(koef)), 1 - np.array(koef), label = "carts of second person")
    ##plt.plot(np.arange(len(power1)), np.array(power1), label = "power carts of first person")
    ##plt.plot(np.arange(len(power1)), 1 - np.array(power1), label = "power carts of second person")
    plt.xlabel("count of all game moves")
    plt.legend()
    plt.show()
   


