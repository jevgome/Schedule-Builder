cmake_minimum_required(VERSION 3.10)

project(Schedule-Builder)

find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(json-c CONFIG REQUIRED)


add_executable(
    Schedule-Builder 
    main.c 
    input/input.c input/input.h 
    input/scraper.c input/scraper.h 
    builder/course.c builder/course.h 
    builder/schedule.c builder/schedule.h
    )

target_link_libraries(Schedule-Builder PRIVATE CURL::libcurl)
target_link_libraries(Schedule-Builder PRIVATE LibXml2::LibXml2)
target_link_libraries(Schedule-Builder PRIVATE json-c::json-c)