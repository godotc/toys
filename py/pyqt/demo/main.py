
from PyQt5.QtWidgets import QApplication, QDialog, QLabel, QMainWindow, QMessageBox, QPushButton, QWidget
import sys


if __name__ == "__main__":
    app = QApplication(sys.argv)
    button = QPushButton('Click')

    def onBtnClick():
        alert = QMessageBox()
        alert.setText("Clicked the button")
        alert.exec_()

    button.clicked.connect(onBtnClick)
    button.show()

    sys.exit(app.exec_())
