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

add_executable(test_find_nalu test_find_nalu.cpp)
target_link_libraries(test_find_nalu zav zpkg pthread)

add_executable(fw fw.cpp)
target_link_libraries(fw zpkg pthread)

add_executable(test_template test_template.cpp)

add_executable(test_zio_loop test_zio_loop.cpp)
target_link_libraries(test_zio_loop zio)

add_executable(small_test small_test.cc)