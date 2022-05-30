#设置Cmake的最低运行版本
cmake_minimum_required (VERSION 3.1)

#——————————————————————————————————————————————————————————#

#该命令表示项目的名称
project (ACESMetadataFormatAnalyst_CPP)

set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR})

# 子项目
add_subdirectory(${TOP_DIR}/Imath-main Imath_binary_dir)
#add_subdirectory(${TOP_DIR}/ctlrender CTL_binary_dir)
#add_subdirectory(${TOP_DIR}/OpenColorIO-main OpenColorIO_binary_dir)


# 需要包含cpp和h的目录
include_directories(/Users/engene_hsuan/Documents/ACESMetadataFormatAnalyst_CPP)
include_directories(/usr/local/include/OpenColorIO)
include_directories(/usr/local/Cellar/python@3.9/3.9.9/Frameworks/Python.framework/Headers)
include_directories(/usr/local/include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(/usr/local/Cellar/pybind11/2.8.1/libexec/lib/python3.9/site-packages)

# 包含库文件的名录
link_directories (/usr/local/lib)
link_directories(/usr/local/lib/python3)
link_directories(/usr/local/Cellar/python@3.9/3.9.9/Frameworks/Python.framework/Versions/3.9/lib)

#——————————————————————————————————————————————————————————#

#设置C++的版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)   #这个是让Cmake兼容qmake的槽函数宏命令
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH  "/Users/youdrew/Qt/6.2.3/macos/lib/cmake")
set(OpenCV_DIR "/usr/local/Cellar/opencv/4.5.3_3/lib/cmake/opencv4")

if(CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#——————————————————————————————————————————————————————————#
#查找与添加
#Qt6Core
find_package(Qt6Widgets)
find_package(Qt6Gui)
find_package(Qt6Core)
find_package(Qt6xml)
find_package(Qt6Charts)
find_package(Qt6 REQUIRED COMPONENTS DataVisualization)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(Boost REQUIRED COMPONENTS)
find_package( OpenCV REQUIRED)

qt6_wrap_ui(ui_FILES amfanalyst.ui)
add_executable(ACESMetadataFormatAnalyst_CPP
    ${ui_FILES}

    "amfanalyst.h"    # 和界面有关的内容
    "ctlrendertool.h" # 渲染管线相关的内容
    "parseamf.h"    # 解析某个amf文件，和amf中有关的内容都储存在这里面
    "scatter.h"     # 用于显示3D色立体的类
    "amfutil.h"    # 关联amf_util python脚本
    "ocioutil.h"    # 关联ocio的库有关的内容
    "callout.h"


    "amfanalyst.cpp"
    "ctlrendertool.cpp"
    "main.cpp"
    "ocioutil.cpp"
    "parseamf.cpp"
    "scatter.cpp"
    "amfutil.cpp"
    "callout.cpp"
)


# 这个是要写在最后面的，前面是找路径，这才是关联
target_link_libraries(ACESMetadataFormatAnalyst_CPP libOpenColorIO.dylib)
target_link_libraries(ACESMetadataFormatAnalyst_CPP Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Xml Qt6::DataVisualization Qt6::Charts)
target_link_libraries(${PROJECT_NAME} ${Python3_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )
