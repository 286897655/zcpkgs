add_library(zcpkgs_capi_static STATIC zcpkgs_api.cc)

target_include_directories(zcpkgs_capi PUBLIC ${ZCF_ROOT}/include/)
