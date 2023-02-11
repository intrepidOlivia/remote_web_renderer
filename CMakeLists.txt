cmake_minimum_required(VERSION 3.10)
project(MyWebRenderer)

set(SOURCES
    src/main.cpp
)

include_directories("C:/Users/marisha/Repositories/my_web_renderer/ultralight/include")

link_directories("C:/Users/marisha/Repositories/my_web_renderer/ultralight/lib")

add_executable(MyWebRenderer ${SOURCES})

target_link_libraries(
    MyWebRenderer 
    Ultralight.lib
    UltralightCore.lib
    WebCore.lib
    AppCore.lib
)