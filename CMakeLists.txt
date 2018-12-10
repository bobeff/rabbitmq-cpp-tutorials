cmake_minimum_required(VERSION 3.12)

project(rabbitmq_test)

include (cmake/conan.cmake)

conan_cmake_run(
  CONANFILE conanfile.txt
  DEBUG_PROFILE "${CMAKE_SOURCE_DIR}/conan_profiles/win_x64_debug.txt"
  RELEASE_PROFILE "${CMAKE_SOURCE_DIR}/conan_profiles/win_x64_release.txt"
  BASIC_SETUP
  CMAKE_TARGETS
  UPDATE)

add_executable(01_send "01/01_send.cpp")
target_link_libraries(01_send CONAN_PKG::SimpleAmqpClient)
set_target_properties(01_send PROPERTIES CXX_STANDARD 17)

add_executable(01_receive "01/01_receive.cpp")
target_link_libraries(01_receive CONAN_PKG::SimpleAmqpClient)
set_target_properties(01_receive PROPERTIES CXX_STANDARD 17)

add_executable(02_send "02/02_send.cpp")
target_link_libraries(02_send CONAN_PKG::SimpleAmqpClient)
set_target_properties(02_send PROPERTIES CXX_STANDARD 17)

add_executable(02_receive "02/02_receive.cpp")
target_link_libraries(02_receive CONAN_PKG::SimpleAmqpClient)
set_target_properties(02_receive PROPERTIES CXX_STANDARD 17)

add_executable(03_send "03/03_send.cpp")
target_link_libraries(03_send CONAN_PKG::SimpleAmqpClient)
set_target_properties(03_send PROPERTIES CXX_STANDARD 17)

add_executable(03_receive "03/03_receive.cpp")
target_link_libraries(03_receive CONAN_PKG::SimpleAmqpClient)
set_target_properties(03_receive PROPERTIES CXX_STANDARD 17)

add_executable(04_send "04/04_send.cpp")
target_link_libraries(04_send CONAN_PKG::SimpleAmqpClient)
set_target_properties(04_send PROPERTIES CXX_STANDARD 17)

add_executable(04_receive "04/04_receive.cpp")
target_link_libraries(04_receive CONAN_PKG::SimpleAmqpClient)
set_target_properties(04_receive PROPERTIES CXX_STANDARD 17)

add_executable(05_send "05/05_send.cpp")
target_link_libraries(05_send CONAN_PKG::SimpleAmqpClient)
set_target_properties(05_send PROPERTIES CXX_STANDARD 17)

add_executable(05_receive "05/05_receive.cpp")
target_link_libraries(05_receive CONAN_PKG::SimpleAmqpClient)
set_target_properties(05_receive PROPERTIES CXX_STANDARD 17)

add_executable(rpc_client "06/rpc_client.cpp")
target_link_libraries(rpc_client CONAN_PKG::SimpleAmqpClient)
set_target_properties(rpc_client PROPERTIES CXX_STANDARD 17)
target_compile_definitions(rpc_client PRIVATE _HAS_AUTO_PTR_ETC)

add_executable(rpc_server "06/rpc_server.cpp")
target_link_libraries(rpc_server CONAN_PKG::SimpleAmqpClient)
set_target_properties(rpc_server PROPERTIES CXX_STANDARD 17)
target_compile_definitions(rpc_server PRIVATE _HAS_AUTO_PTR_ETC)
