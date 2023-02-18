cmake_minimum_required(VERSION 3.8)

add_executable(EngineBackendTests
        StartupManagerTest.cxx
)


add_test(
        NAME EngineBackendTests
        COMMAND EngineBackendTests
)

target_include_directories(EngineBackendTests
        PRIVATE
        ..
)

target_link_libraries(EngineBackendTests
        gtest
        gtest_main
        dachshund_direct_engine
)
