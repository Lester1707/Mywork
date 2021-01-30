class person:
    def __init__(self, id, name, sex):
        self.id = id;
        self.name = name;
        self.sex = sex;

    def print_p(self):
        print(self.id)
        print(self.name)
        print(self.sex)

class femaly:
    def __init__(self, id):
        self.id = id
        self.parents = []
        self.children = []

    def add_par(self, person):
        self.parents.append(person)

    def add_chil(self, person):
        self.children.append(person)

    def ind(self):
        return self.id

    def print_fem(self):
        print(self.id)
        print("parents:")
        for i in range(len(self.parents)):
            self.parents[i].print_p()
        print("childs:")
        for i in range(len(self.children)):
            self.children[i].print_p()

def print_femals(femalies):
    for i in range(len(femalies)):
        print("femaly ", i, ":")
        femalies[i].print_fem()
        print(" ")

file = open('roal.ged', 'r')
lines = file.readlines()

femalies = []
persons = []
id = []
name = []
sex = 'U'
FEMS = []
FEMC = []

for i in lines:
    if i == lines[len(lines) - 1]:
        if len(name) != 0 and len(id) != 0 and sex != 'U' and (len(FEMS) != 0 or len(FEMC) != 0):
            persons.append(person(id, name, sex))
            for q in range(len(femalies)):
                for j in range(len(FEMC)):
                    if FEMC[j] == femalies[q].ind():
                       femalies[q].add_chil(person(id, name, sex))
                       FEMC[j] = []
                for j in range(len(FEMS)):
                    if FEMS[j] == femalies[q].ind():
                        femalies[q].add_par(person(id, name, sex))
                        FEMS[j] = []
            if len(FEMS) != 0:
                for q in range(len(FEMS)):
                    if len(FEMS[q]) != 0:
                        f1 = femaly(FEMS[q])
                        f1.add_par(person(id, name, sex))
                        femalies.append(f1)
                        FEMS[q] = []
            if len(FEMC) != 0:
                for q in range(len(FEMC)):
                    if len(FEMC[q]) != 0:
                        f2 = femaly(FEMC[q])
                        f2.add_chil(person(id, name, sex))
                        femalies.append(f2)
                        FEMC[q] = []   
            id = []
            name = []
            sex = 'U'
    if i[0:3:1] == "0 @":
        if len(name) != 0 and len(id) != 0 and sex != 'U' and (len(FEMS) != 0 or len(FEMC) != 0):
            persons.append(person(id, name, sex))
            for q in range(len(femalies)):
                for j in range(len(FEMC)):
                    if FEMC[j] == femalies[q].ind():
                       femalies[q].add_chil(person(id, name, sex))
                       FEMC[j] = []
                for j in range(len(FEMS)):
                    if FEMS[j] == femalies[q].ind():
                        femalies[q].add_par(person(id, name, sex))
                        FEMS[j] = []
            if len(FEMS) != 0:
                for q in range(len(FEMS)):
                    if len(FEMS[q]) != 0:
                        f1 = femaly(FEMS[q])
                        f1.add_par(person(id, name, sex))
                        femalies.append(f1)
                        FEMS[q] = []
            if len(FEMC) != 0:
                for q in range(len(FEMC)):
                    if len(FEMC[q]) != 0:
                        f2 = femaly(FEMC[q])
                        f2.add_chil(person(id, name, sex))
                        femalies.append(f2)
                        FEMC[q] = []   
            id = []
            name = []
            sex = 'U'
        j = 4
        while i[j] != "@":
            id.append(i[j])
            j += 1
        j = 0
    if i[0:7:1] == "1 NAME " and len(id) != 0:
        j = 7
        while i[j] != "\n":
            name.append(i[j])
            j += 1
        j = 0
    
    if i[0:6:1] == "1 SEX " and len(name) != 0:
        sex = i[6]

    if i[0:9:1] == "1 FAMS @F" and len(name) != 0:
        some_l1 = []
        j = 9
        while i[j] != "@":
            some_l1.append(i[j])
            j += 1
        FEMS.append(some_l1)
        
    if i[0:9:1] == "1 FAMC @F" and len(name) != 0:
        some_l2 = []
        j = 9
        while i[j] != "@":
            some_l2.append(i[j])
            j += 1
        FEMC.append(some_l2)

for i in range(len(femalies)):
    if len(femalies[i].children) != 0 and len(femalies[i].parents) != 0:
        for j in range(len(femalies[i].parents)):
            for q in range(len(femalies[i].children)):
                if femalies[i].parents[j].sex == 'M':
                    print("father(", end = "")
                    for p in range(len(femalies[i].parents[j].name)):
                        if femalies[i].parents[j].name[p] != '/':
                            print(femalies[i].parents[j].name[p], end = "")
                    print("; ", end = "")
                    for p in range(len(femalies[i].children[q].name)):
                        if femalies[i].children[q].name[p] != '/':
                            print(femalies[i].children[q].name[p], end = "")
                    print(")")

for i in range(len(femalies)):
    if len(femalies[i].children) != 0 and len(femalies[i].parents) != 0:
        for j in range(len(femalies[i].parents)):
            for q in range(len(femalies[i].children)):
                if femalies[i].parents[j].sex == 'F':
                    print("mother(", end = "")
                    for p in range(len(femalies[i].parents[j].name)):
                        if femalies[i].parents[j].name[p] != '/':
                            print(femalies[i].parents[j].name[p], end = "")
                    print("; ", end = "")
                    for p in range(len(femalies[i].children[q].name)):
                        if femalies[i].children[q].name[p] != '/':
                            print(femalies[i].children[q].name[p], end = "")
                    print(")")                       

#print_femals(femalies)

