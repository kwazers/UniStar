cmake_minimum_required(VERSION 3.22)
project("vpnconfigurator")

# Путь к вашей libTor.so
set(TOR_LIB_PATH ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libTor.so)

# Объявляем библиотеку как IMPORTED
add_library(tor SHARED IMPORTED)
set_target_properties(tor PROPERTIES
    IMPORTED_LOCATION ${TOR_LIB_PATH}
)

# Ваш C++ код
add_library(vpn_engine SHARED
    src/main/cpp/vpn_engine.cpp
    src/main/cpp/tor_manager.cpp
)

# Подключаем заголовки и линкуем
target_include_directories(vpn_engine PRIVATE
    ${CMAKE_SOURCE_DIR}/src/main/jniLibs/include
)

target_link_libraries(vpn_engine
    tor          # ← ваша libTor.so
    log          # Android logging
    c++_shared
)