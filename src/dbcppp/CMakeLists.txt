
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

file(GLOB header
    "*.h"
)
file(GLOB src
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${header} ${src})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME})
#target_link_libraries(${PROJECT_NAME}  ${Boost_LIBRARIES})

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR})

