cmake_minimum_required(VERSION 3.8)
project(keyboard_teleop)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(include ${SDL2_INCLUDE_DIRS})

add_executable(keyboard_teleop_diff src/keyboard_teleop_diff.cpp)
ament_target_dependencies(keyboard_teleop_diff rclcpp geometry_msgs)
target_link_libraries(keyboard_teleop_diff ${SDL2_LIBRARIES})

add_executable(keyboard_teleop_omni src/keyboard_teleop_omni.cpp)
ament_target_dependencies(keyboard_teleop_omni rclcpp geometry_msgs)
target_link_libraries(keyboard_teleop_omni ${SDL2_LIBRARIES})

install(TARGETS 
  keyboard_teleop_diff
  keyboard_teleop_omni
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
