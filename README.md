# Wiring for ROS 2
![GitHub](https://img.shields.io/github/license/polyhobbyist/wiring_for_ros)
![Twitter URL](https://img.shields.io/twitter/follow/ooeygui)
![YouTube Video Views](https://img.shields.io/youtube/views/6eoiakuSA-w)

## About 
Have you ever wanted to use an [Arduino](http://arduino.cc) libraries on a Raspberry Pi or Jetson Nano?
Arduino libraries are written in C++, although traditionally it was called *Wiring*, which is derived from the Processing language. The Arduino exposes application programming interfaces (APIs) which are a beautiful abstraction over physical electronics interfaces. 

This codebase implements many of the Arduino APIs needed to run many Arduino libraries on a Raspberry Pi, Jetson nano, or any Linux CPU with electronics busses exposed via low level linux APIs. 

> Although care has been taken to minimize code change in the Arduino libraries, some are unfortunately unavoidable due to the differences between applications running on a CPU and firmware running on a Microcontroller.

## Using
This library is designed to be a "Git submodule" of a ROS 2 package, which sits next to the Arduino library which you'd like to use in your ROS node.

When you create your ROS Package in a colcon workspace, it should eventually be laid out like this - I'll show you how in the next step:

```bash
my_ros_node_which_uses_wiring
├── CMakeLists.txt
├── LICENSE
├── package.xml
├── README.md
└── src
    ├── main.cpp
    ├── excellent_arduino_library
    │   └── src
    └── wiring_for_ros
```

`my_ros_node_which_uses_wiring` is your ROS node, which can be [bootstrapped using ](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Creating-Your-First-ROS2-Package.html) `ros2 pkg create`. 

### Submodules
The arduino library and wiring for ros are designed to be included as git submodules, because they are tightly coupled to the ROS node. (If they weren't so tight, I'd recommend using a [vcs and repos file](https://wiki.ros.org/vcstool)).

You'll likely need to make changes to the arduino library to support CPU idyosynchrosies. I recommend creating a [fork of the library](https://docs.github.com/en/get-started/quickstart/fork-a-repo) and using that repo url for the next step.

`cd` into the src of `my_ros_node_which_uses_wiring`, then run the following commands
```bash

git submodule add https://github.com/polyhobbyist/wiring_for_ros
git submodule add https://github.com/excellent_library_author/excellent_arduino_library
```

### Modify CMakeFiles.txt
Now for the hard part. Modify `CMakeFiles.txt` to reference the sources files and headers of both `wiring_for_ros` and the `excellent_arduino_library`. The exact updates unfortunately are specific to the library you are including.

```cmake
include_directories(
  include/${PROJECT_NAME}
  ${SOURCE_DIR}/include
  src/wiring_for_ros/include
  src/excellent_arduino_library/include
  src/excellent_arduino_library/src
)

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/wiring_for_ros/SPI.cpp
  src/wiring_for_ros/Wire.cpp
  src/wiring_for_ros/Arduino.cpp
  src/excellent_arduino_library/src/file1.cpp
  src/excellent_arduino_library/src/file2.cpp
)
```

Finally, Wiring for ROS makes use of Linux modules and helper libraries to function. So these need to be added to your `CMakeLists.txt` (This is subject to change as more features are added):

```cmake
find_program(MAKE_EXECUTABLE
    NAMES gmake mingw32-make make
    NAMES_PER_DIR
    DOC "GNU Make")
include(ExternalProject)
ExternalProject_Add(libi2c
    GIT_REPOSITORY https://github.com/amaork/libi2c.git
    UPDATE_DISCONNECTED true
    BUILD_IN_SOURCE true
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${MAKE_EXECUTABLE}
    INSTALL_COMMAND ""
)
ExternalProject_Get_property(libi2c SOURCE_DIR)

message (STATUS "*** libi2c installed to ${SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} ${SOURCE_DIR}/libi2c.so  gpiod)
add_dependencies(${PROJECT_NAME}  libi2c) 
```

# Supported Electrical Busses

|Bus | Arduino API | Notes|
|---|---|---|
|I2C|Wire class|The Wiring API is present in the main repo, and usable for many device types
|SPI|SPI class|Coming Soon|
|GPIO|digitalWrite/digitalRead| Write is available in the gpio repo, Read coming soon.|
|ADC|analogWrite/analogRead| Not directly supported with a Pi header; investigating support over i2c.|


# Referencing
If you find this project useful, please link back. 








