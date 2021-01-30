import sys
import random as ran
from PyQt5.QtWidgets import (QWidget, QProgressBar,
    QPushButton, QApplication, QLabel,  QLineEdit, QCheckBox, QInputDialog)
from PyQt5.QtCore import QBasicTimer

class Example(QWidget):

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.x = 0
        self.y = 0

        self.c = 0
        self.k = 1
        self.i = 0
        self.value = 0

        self.lbl = QLabel(self)
        self.lbl.move(60, 100)
        self.lbl.setText("У тебя 0 кликов!")

        self.lbl2 = QLabel(self)
        self.lbl2.move(250, 253)
        self.lbl2.setText("Всего пройдено " + str(self.value) + " уровней")

        self.lbl3 = QLabel(self)
        self.lbl3.move(150, 150)
        self.lbl3.setText("Скорость = 0 (кликов в секунду)")

        self.bar = QProgressBar(self)
        self.bar.setGeometry(25, 250, 200, 25)

        clicker = QPushButton("-> Click <-", self)
        clicker.move(50, 70)
        clicker.clicked.connect(self.doAction)

        self.timer = QBasicTimer()
        self.step = 0

        self.setGeometry(300, 300, 500, 300)
        self.setWindowTitle('Cliker v1.0')
        self.show()

    def doAction(self):
        self.x = ran.randrange(0, 300, 1)
        self.y = ran.randrange(0, 300, 1)
        if self.y == 0:
            self.lbl4.move(self.x, self.y ) 
        self.i +=1
        self.timer.start(10000/self.i, self)
        self.lbl3.setText("Скорость = " + str(round(96/(10000/self.i), 5)) + " (кликов в секунду)")
        self.lbl3.adjustSize()
        self.lbl.setText("У тебя " + str(self.i)  + " кликов!")
        self.lbl.adjustSize()
        if self.c/self.k != 100:
            self.c = self.c + 1
            self.bar.setValue(self.c/self.k)
        else:
            self.value += 1
            self.lbl2.setText("Всего пройдено " + str(self.value) + " уровней")
            self.lbl2.adjustSize()
            self.c = 0
            self.k = self.k*10
            self.bar.setValue(0)
    
    def timerEvent(self, e):
        self.lbl3.setText("Скорость = " + str(round(96/(10000/self.i), 5)) + " (кликов в секунду)")
        self.lbl3.adjustSize()
        self.lbl.setText("У тебя " + str(self.i)  + " кликов!  ")
        self.lbl.adjustSize()
        if self.c/self.k != 100: 
            self.i = self.i + 1
            self.c = self.c + 1
            self.bar.setValue(self.c/self.k)
        else:
            self.value += 1
            self.lbl2.setText("Всего пройдено " + str(self.value) + " уровней")
            self.lbl2.adjustSize()
            self.c = 0
            self.bar.setValue(0)
            self.k = self.k*10

if __name__ == '__main__':

    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())