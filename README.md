# Data Visualization Application Based on Qt
## 1. Introduction
### The application is a Qt-based GUI application which allows the user to import different datasets, plot them in the application, and allow the user to create a function from a combination of the datasets. To handle the plot, the library QCustomPlot is used.
## 2. Implementation
### *2.1 DataProcessApp Class*
### This class is the main window of the application derived from QMainWindow class. Every action that users can take is triggered from the tool bar and the context menu in this window.
### *2.2 DataSelectionDialog Class*
### This class implements the dialog for users to select data sets to plot. It's derived from QDialog class.
### *2.3 ExceptionDialog Class*
### This class implements the dialog that is particularly used to show error messages if users load datasets with wrong format. It's derived from QDialog class.
### *2.4 WriteFunctionDialog Class*
### This class implements the dialog for users to write function from a combination of datasets. It's derived from QDialog class.
## 3. User Instruction
### *3.1 Loading Files*
### To start with the application, users need to load datasets into the program. Users can load as many file as they want in every loading operation and all previous loaded files would be retained. If the same file is loaded for a second time, the latest loading will override the previous loading. Just remember to press and hold the “Ctrl” button when selecting multiple files in the File dialog. To load datasets, users can just click the “Load Datasets” icon in the menu bar, tool bar or context menu. This application has provided three demo files: “wave1.txt” “wave2.txt” “wave3.txt”, please find them in the project file. If a file with incorrect content format is selected, an error message will show up. The user should then amend the content of the file and load it again.
### Acceptable datasets in this application are expected to be in a .txt file. Every key-value pair should be in a separate row. The key and the value should be decimals or integers and be separated by a comma with no space before, in the middle, or after them (e.g. 2,3).
### *3.2 Selecting Datasets to Plot*
### The next step is to select among loaded datasets to plot them on the screen. Users can do it in the menu bar, tool bar or context menu. After clicking the icon, a new dialog will show up and the user can select arbitrary number of datasets to plot. To select multiple items, just click them one by one or press and hold the left button of the mouse and move it over the items.
### *3.3 Writing Function for Plots*
### If the user selects more than one dataset in the previous step, the application will automatically show up a dialog and ask the user to write a function applying to the datasets been selected. The user can either accept this dialog by entering the available function or reject it and write the function later when needed. This action can be accessed any time in the tool bar or in the context menu.
### *3.4 Changing the Colour of Graphs*
### There are three ways of changing graph colors.
### (1)Select a single graph by single click and then click the right mouse button. Then Change Graph Color action is there in the context menu and a QColorDialog will show up after this action is triggered. Users can select multiple graphs holding the “Ctrl” button.
### (2)Double click on a graph, then the QColorDialog will automatically appear and then users can pick colours in it.
### (3)Select one or multiple graphs by pressing and holding “Ctrl” button and clicking graphs one by one. Then click the Change Color icon in the tool bar.
### *3.5 Other Operations*
### This application provides additional actions which will help users to manipulate it more easily. These actions are all listed in the menu bar and context menu.
### (1)Delete All Datasets and Graphs
### This is actually a reset operation. All loaded files and plotted graphs will be deleted from the application. Users can use this function if they want a new group of datasets to be loaded and plotted.
### (2)Remove the Function Graph
### This action will delete the function graph written by the user from the screen. The screen will readjust its size to fit all graphs properly.
### (3)Clean the Screen
### This is just an quick action for users to clear all graphs on the screen.
### (4)Restore to Original View
### Users can use mouse to zoom and drag the interface. This function is used to go back to the initial view after users have zoomed in and out or have dragged plots.
## 4. Run the Application
### IDE Version: Qt Creator 8.0.2 (or any other version that can run the application)
### For more information, please see [https://doc.qt.io/](https://doc.qt.io/) and [https://www.qcustomplot.com/](https://www.qcustomplot.com/).