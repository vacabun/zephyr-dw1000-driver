zephyr_library()
zephyr_library_sources(
    src/deca_port.c 
    src/dw1000_hw.c 
    src/dw1000_spi.c)
zephyr_include_directories(inc)
