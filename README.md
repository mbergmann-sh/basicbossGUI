# basicbossGUI
BasicBoss GUI is a Qt5 interface to the well-known CBM-Basic 2.0 Compiler "Basic Boss" for the Commodore C64. It runs on Windows, but should be easily portable to Linux with a working WINE configuration.

# Warning
This code still contains some bugs! The app uses german language - there are no translations available yet.

# LICENSE
Copyright (C) 2018/2019 by Michael Bergmann. Placed in the public domain with some restrictions!

These files are part of my Basic Boss GUI interface, using classes of the Qt5 toolkit.

You may use or enhance this piece of software anyway you want to - as long as you don't violate laws or copyright issues. I hereby explicitely prohibit the usage of my work to people who believe in racism, fascism and any kind of attitude against democratic lifestyle. It is self-explanatory that this prohibits the usage of my work to any member or fan of the german AfD party.

Further information about Qt licensing is available at: http://www.trolltech.com/products/qt/licensing.html or by contacting info@trolltech.com.


This program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

# FEATURES
- load and compile CBM-Basic v2.0 .prg files
- configurable compiler and emulation paths
- start VICE CBM64 emulator
- show compiler output

# Compiling the app...
...should work fine under Linux, Windows, and probably under BSD, too (didn't test that!).
You'll need a computer with either MSVC, GCC or MinGW C/C++ compiler setup with Qt5 installed and working!

1. Create a folder to store the app
2. CD to this folder
3. git clone https://github.com/mbergmann-sh/basicbossGUI.git
4. If on Windows, open a Qt command prompt correspondending to the compiler in use. ANY system: CD to <your_folder>/basicbossGUI
5. qmake application.pro (creates neccessary descriptions for compiling and linking)
6. qmake (creates Makefiles for Debug and Release versions)
7. MSVC: nmake, GCC or MinGW: make -j4
8. Wait for compiler to finish

The easy way: Simply import the .pro file into Qt Creator, configure your toolchain and build the app... ;)

# Making the app run standalone on Windows
1. Open a Qt command prompt correspondending to the compiler in use 
2. CD to folder where the release version was stored
3. windeployqt --angle --compiler-runtime --release BasicBoss.exe (this will create some additional folders and copy most of the needed DLLs to the folder containing your executable)
4. Manually copy appropriate Qt5PrintSupport.dll to that folder
5. Add an entry containing the path to that folder to your system's $PATH variable if you want to call the app from command line!

# Usefull links for Qt5 coders
- Qt5 open source edition download site: https://www.qt.io/download-qt-installer?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5
- Qt5 documentation: http://doc.qt.io/qt-5/ 
