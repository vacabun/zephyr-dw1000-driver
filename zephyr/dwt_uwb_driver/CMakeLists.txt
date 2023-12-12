zephyr_library()
zephyr_library_sources(
    src/deca_device.c 
    src/deca_params_init.c
    src/deca_range_tables.c)
zephyr_include_directories(inc)
