cmake_minimum_required(VERSION 3.16)
project(NIPD_ABM LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(NIPD_ABM src/main.cpp src/Agent.hpp src/Agent.cpp src/Saver.hpp src/Saver.cpp src/constants.hpp)
target_link_libraries(NIPD_ABM PRIVATE sfml-graphics)
target_compile_features(NIPD_ABM PRIVATE cxx_std_17)

add_subdirectory(src/AKML-lib/)
target_link_libraries(NIPD_ABM PRIVATE AKML)
target_include_directories(NIPD_ABM PRIVATE src/AKML-lib/)
set_property(TARGET AKML PROPERTY CXX_STANDARD 20)

if(WIN32)
    add_custom_command(
        TARGET NIPD_ABM
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS NIPD_ABM)
