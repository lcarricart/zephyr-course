zephyr_library()
zephyr_library_sources(init.c)

if(CONFIG_NXP_RW6XX_BOOT_HEADER)
  zephyr_compile_definitions(BOARD_FLASH_SIZE=CONFIG_FLASH_SIZE*1024)
  # This FCB is specific to the flash on this board, it won't work
  # for boards with different flash chips. If you flash this FCB
  # onto a board with a different flash chip you may break it.
  # See MCUXpresso config tools for making a correct one.
  zephyr_library_sources(W25Q512JVFIQ_FCB.c)
endif()

# I thought I should put it here but positioning in this file does not mean that init.c executes before my_board_init.c or something
# Execution sequence depends on things such as the parameters in the SYS_INIT() function
# zephyr_library_sources(my_board_init.c)

# Then, I realized that since my_board_init.c is an application file in my project, it could be better included inside the app's CMakeLists.txt