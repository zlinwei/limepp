add_library(LimeSDR Transmitter.cpp LimeDevice.cpp)

target_link_libraries(LimeSDR ${LimeSuite_LIBRARIES})