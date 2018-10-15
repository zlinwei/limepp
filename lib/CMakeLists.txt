add_library(LimeSDR Transmitter.cpp LimeDevice.cpp)

target_link_libraries(LimeSDR ${Boost_LIBRARIES} ${LimeSuite_LIBRARIES} pthread)