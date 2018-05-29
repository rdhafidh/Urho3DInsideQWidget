# Urho3DInsideQWidget
===========

 A simple prototype of Urho3D scene content in Qt5 QWidget. This project provides out of the box
 integration of Urho3D scene in Qt5 Widget. Some notable information that led me to write this 
 code is from [dakilla's answer](https://discourse.urho3d.io/t/integration-with-qt5-7/2393/8)
 and my personal possible crash fix improvements. 
 
 This project is currently tested on windows 7, MSVC 2017 Community and Qt version 5.9. There are several notes to consider:
 
 1. You must edit urhoqt1.pro and change URHO_HOME variable to point to your chroot of your urho3d instalation folder.
 2. You must edit [engineParameters_ ResourcePrefixPaths](https://github.com/rdhafidh/Urho3DInsideQWidget/blob/master/urho3dwidget.cpp#L175) to point to your  URHO_HOME/share/Resources folder.
 
Happy Hacking !
 
![](https://raw.githubusercontent.com/rdhafidh/Urho3DInsideQWidget/master/images/demo.png)

This application license is under **MIT License**
 
 
 
 
 
 