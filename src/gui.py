from genericpath import exists
from PyQt5 import QtCore, QtWidgets
from PyQt5.QtWidgets import QFileDialog
from os import system,remove
from os.path import exists

compilerRunLocation = "./bin/compiler.exe"

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        self.MainWindow = MainWindow
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1034, 793)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.CodeAreaWidget = QtWidgets.QWidget(self.centralwidget)
        self.CodeAreaWidget.setGeometry(QtCore.QRect(20, 30, 491, 691))
        self.CodeAreaWidget.setObjectName("CodeAreaWidget")
        self.CodeAreaPlainTextEdit = QtWidgets.QPlainTextEdit(self.CodeAreaWidget)
        self.CodeAreaPlainTextEdit.setGeometry(QtCore.QRect(13, 9, 471, 671))
        self.CodeAreaPlainTextEdit.setObjectName("CodeAreaPlainTextEdit")
        self.symbolTabelWidget = QtWidgets.QWidget(self.centralwidget)
        self.symbolTabelWidget.setGeometry(QtCore.QRect(539, 29, 471, 191))
        self.symbolTabelWidget.setObjectName("symbolTabelWidget")
        self.textBrowser = QtWidgets.QTextBrowser(self.symbolTabelWidget)
        self.textBrowser.setGeometry(QtCore.QRect(0, 0, 471, 191))
        self.textBrowser.setObjectName("textBrowser")
        self.outErrWidget = QtWidgets.QWidget(self.centralwidget)
        self.outErrWidget.setGeometry(QtCore.QRect(540, 240, 471, 191))
        self.outErrWidget.setObjectName("outErrWidget")
        self.textBrowser_4 = QtWidgets.QTextBrowser(self.outErrWidget)
        self.textBrowser_4.setGeometry(QtCore.QRect(0, 0, 471, 191))
        self.textBrowser_4.setObjectName("textBrowser_4")
        self.QuadWidget = QtWidgets.QWidget(self.centralwidget)
        self.QuadWidget.setGeometry(QtCore.QRect(540, 450, 471, 191))
        self.QuadWidget.setObjectName("QuadWidget")
        self.textBrowser_5 = QtWidgets.QTextBrowser(self.QuadWidget)
        self.textBrowser_5.setGeometry(QtCore.QRect(0, 0, 471, 191))
        self.textBrowser_5.setObjectName("textBrowser_5")
        self.CompileButton = QtWidgets.QPushButton(self.centralwidget)
        self.CompileButton.setGeometry(QtCore.QRect(720, 660, 89, 25))
        self.CompileButton.setObjectName("CompileButton")
        self.CodeAreaLabel = QtWidgets.QLabel(self.centralwidget)
        self.CodeAreaLabel.setGeometry(QtCore.QRect(40, 10, 67, 17))
        self.CodeAreaLabel.setObjectName("CodeAreaLabel")
        self.SymbolTableLabel = QtWidgets.QLabel(self.centralwidget)
        self.SymbolTableLabel.setGeometry(QtCore.QRect(550, 10, 91, 17))
        self.SymbolTableLabel.setObjectName("SymbolTableLabel")
        self.OutputErrorLable = QtWidgets.QLabel(self.centralwidget)
        self.OutputErrorLable.setGeometry(QtCore.QRect(550, 220, 101, 17))
        self.OutputErrorLable.setObjectName("OutputErrorLable")
        self.QuadruplesLabel = QtWidgets.QLabel(self.centralwidget)
        self.QuadruplesLabel.setGeometry(QtCore.QRect(550, 430, 81, 17))
        self.QuadruplesLabel.setObjectName("QuadruplesLabel")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1034, 22))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtWidgets.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionOpen_File = QtWidgets.QAction(MainWindow)
        self.actionOpen_File.setObjectName("actionOpen_File")
        self.actionSave_File = QtWidgets.QAction(MainWindow)
        self.actionSave_File.setObjectName("actionSave_File")
        self.menuFile.addAction(self.actionOpen_File)
        self.menuFile.addAction(self.actionSave_File)
        self.menubar.addAction(self.menuFile.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.CodeAreaPlainTextEdit.setPlainText(_translate("MainWindow", "void main(){\n"
"    return;\n"
"}"))
        self.CompileButton.setText(_translate("MainWindow", "Compile"))
        self.CodeAreaLabel.setText(_translate("MainWindow", "Code"))
        self.SymbolTableLabel.setText(_translate("MainWindow", "Symbol Table"))
        self.OutputErrorLable.setText(_translate("MainWindow", "Output Errors"))
        self.QuadruplesLabel.setText(_translate("MainWindow", "Quadruples"))
        self.menuFile.setTitle(_translate("MainWindow", "Fi&le"))
        self.actionOpen_File.setText(_translate("MainWindow", "Open File"))
        self.actionSave_File.setText(_translate("MainWindow", "Save File"))
        self.actionOpen_File.triggered.connect(self.openFileAction)
        self.CompileButton.clicked.connect(self.compileButtonAction)

    def openFileAction(self):
        print("openFileAction")
        fname = QFileDialog.getOpenFileName(self.MainWindow, "Open File", '',"All Files (*)")
        self.CodeAreaPlainTextEdit.setPlainText(open(fname[0]).read())
    
    def compileButtonAction(self):
        print("compileButtonAction")
        if(not exists(compilerRunLocation)) :
            print("compilerRunLocation not exists")
            system("make")
        fileData = self.CodeAreaPlainTextEdit.toPlainText()
        textFile = open("temp.c", "w")
        textFile.write(fileData)
        textFile.close()
        system(f"{compilerRunLocation} temp.c")
        remove("temp.c")
        self.outputQuads()
        self.outputErrors()
        self.outputSymTable()
    
    def outputQuads(self):
        print("outputQuads")
        self.textBrowser_5.setPlainText(open("output.asm").read())

    def outputErrors(self):
        print("outputErrors")
        #self.textBrowser_4.setPlainText(open("output.err").read())
        self.textBrowser_4.setPlainText("Errors and Warnings")

    def outputSymTable(self):
        print("outputSymTable")
        self.textBrowser.setPlainText(open("symbol_table.txt").read())
        #self.textBrowser.setPlainText("Symbol Table")

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())