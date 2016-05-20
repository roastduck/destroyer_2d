BUILD
=======

Dependencies
-------

This program makes use of *LiquidFun* which provides 2D physics simulation, and *GLFW* is an *OpenGL* loader. Both of them are included in the `third_party` directory, so you don't have to install them by yourself. But you have to install **their** dependencies as listed below:

***For Linux***

- cmake (>= 2.8.12.1)
- OpenGL: libglapi-mesa (>= 8.0.4)
- Xlib: libx11-dev
- libXi: libxi-dev
- xorg-dev
- libgl1-mesa-dev

If you use `apt-get`, just type

    sudo apt-get install cmake libglapi-mesa libx11-dev libxi-dev xorg-dev libgl1-mesa-dev

Please go to section **Building**

***For Windows***

- cmake (>= 2.8.12.1)
- Windows (>= 7)
- Visual Studio (2010 or 2012)

You can download them from their official sites.

Please go to section **Building**

***For OS X***

- cmake (>= 2.8.12.1)
- Xcode (>= 5.0.1)
- Xquartz (>= 2.7.5) (xorg-server >= 1.14.4)

Building
-------

***For Linux***

Execute

    cmake .
    make

You will get executable `bin/Destroyer2D`.

If you want to build in Debug mode, execute

    cmake -DCMAKE_BUILD_TYPE=Debug
    make

If you want to switch back to Release mode, execute

    cmake -DCMAKE_BUILD_TYPE=Release
    make

***For Windows***

For Visual Studio 2012, execute

    cmake -G "Visual Studio 11"

For Visual Studio 2010, execute

    cmake -G "Visual Studio 10"

If you run cmake under Cygwin, you need to unset temp variables as

    ( unset {temp,tmp,TEMP,TMP} ; cmake -G "Visual Studio 11" )

or

    ( unset {temp,tmp,TEMP,TMP} ; cmake -G "Visual Studio 10" )

**Then**, double-click on `Destroyer2D.sln` to open the solution. Select "Build -> Build Solution" from the menu.

You will get executable `bin/Destroyer2D`.

***For OS X***

Execute

    cmake -G "Xcode"

Double-click on `Destroyer2D.xcodeproj` to open the project in Xcode. Select "Product -> Build" from the menu.

You will get executable `bin/Destroyer2D`.
