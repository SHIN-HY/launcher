cmake_minimum_required(VERSION 3.8)
project(launcher)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosgraph_msgs REQUIRED)
find_package(roscco_msgs REQUIRED)
find_package(adma_ros_driver_msgs REQUIRED)
find_package(autoware_auto_vehicle_msgs REQUIRED)

add_executable(health_checker src/health_check.cpp)

ament_target_dependencies(
  health_checker 
  rclcpp
  rosgraph_msgs
  roscco_msgs
  adma_ros_driver_msgs
  autoware_auto_vehicle_msgs
  )

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS 
  health_checker
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
