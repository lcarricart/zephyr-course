message(STATUS "Course LED module discovered")
zephyr_include_directories(include)
add_subdirectory(drivers/course-led)
