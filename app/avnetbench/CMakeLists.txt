set(ASIO_ROOT ${ZCF_ROOT}/3rdparty/asio-1.28.0)

INCLUDE_DIRECTORIES(${ASIO_ROOT}/include/)

add_executable(asioavnet asioavnet.cc)

target_link_libraries(asioavnet pthread zpkg)
