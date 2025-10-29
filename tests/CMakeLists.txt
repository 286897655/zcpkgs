add_executable(g7221 g7221.cpp)
target_link_libraries(g7221 zav pthread)

add_executable(g711 g711.cpp)
target_link_libraries(g711 zav pthread)

add_executable(g722 g722.cpp)
target_link_libraries(g722 zav pthread)

add_executable(audio_convert audio_convert.cpp)
target_link_libraries(audio_convert zav pthread)

if(ARCH_AMD64)
    add_executable(x86_simd x86_simd.cpp)
    target_compile_options(x86_simd PUBLIC -mavx2)
    target_link_libraries(x86_simd zpkg pthread)
endif()

if(ARCH_AARCH64)
    add_executable(arm_simd arm_simd.cpp)
    target_link_libraries(arm_simd zpkg pthread)
endif()

add_executable(bench bench.cpp)
target_link_libraries(bench zpkg pthread)

add_executable(bench_h26x_find bench_h26x_find.cpp)
target_link_libraries(bench_h26x_find zav zpkg pthread)

add_executable(fw fw.cpp)
target_link_libraries(fw zpkg pthread)

add_executable(test_template test_template.cpp)

add_executable(test_event_loop test_event_loop.cpp)
target_link_libraries(test_event_loop zio)

add_executable(small_test small_test.cc small_a.cc)
target_link_libraries(small_test zpkg)

add_executable(test_log test_log.cc)
target_link_libraries(test_log zpkg)

add_executable(thread_priority thread_priority.cc)
target_link_libraries(thread_priority zpkg pthread)

add_executable(hash_test hash_test.cc)
target_link_libraries(hash_test zpkg pthread)

add_executable(time_test time_test.cc)
target_link_libraries(time_test zpkg pthread)