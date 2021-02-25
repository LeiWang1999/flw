cmake_minimum_required(VERSION 3.0)

project(ape_unflod)


include_directories("include")
include_directories("include/ape")
include_directories("src")


add_definitions(-fPIC)

set(SOURCE
            app/main.c
            src/ape_init.c
            src/ape_forward.c



            src/ape/conv.c
            src/ape/sdp.c
            src/ape/cdp.c
            src/ape/pdp.c
            src/ape/bdma.c
            src/ape/rbk.c
	    src/ape/common.c
	    src/ape/engine.c
        
            src/ape/engine_data.c
            src/ape/ape_debug.c
            src/data/surface_dat.c
            src/data/op_dat.c
	    src/data/dma_address_dat.c
)


add_executable(ape_unflod ${SOURCE})





