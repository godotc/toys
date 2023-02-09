
add_executable(AnnexBExample)
target_sources(AnnexBExample PRIVATE main.cc)
target_include_directories(AnnexBExample PRIVATE ${CMAKE_SOURCE_DIR}/include/AnnexB)