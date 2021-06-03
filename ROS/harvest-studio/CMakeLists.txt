cmake_minimum_required(VERSION 2.8.3)

project(harvest-studio)

find_package(catkin REQUIRED COMPONENTS
  cmake_modules
  dynamixel_workbench_msgs
  dynamixel_workbench_toolbox
  geometry_msgs
  roscpp
  sensor_msgs
  std_msgs
  std_srvs
  trajectory_msgs
)

catkin_package()

find_package(roslaunch)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

foreach(dir config launch meshes urdf)
	install(DIRECTORY ${dir}/
		DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach(dir)

add_executable(camera2dynamixel src/camera2dynamixel.cpp)
add_dependencies(camera2dynamixel ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(camera2dynamixel ${catkin_LIBRARIES} ${YAML_CPP_LIBRARIES})