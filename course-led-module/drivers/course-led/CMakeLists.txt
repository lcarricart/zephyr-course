if (CONFIG_COURSE_LED)
    target_sources(app PRIVATE course_led.c)
    target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()
