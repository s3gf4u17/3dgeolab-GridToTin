set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")

cmake_minimum_required(VERSION 3.1...3.23)

project( GridToTin )

find_package( CGAL QUIET COMPONENTS  )
if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  
endif()

find_package( Boost REQUIRED )
if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()  
endif()

find_package( Eigen3 REQUIRED )

#add_subdirectory(library/tinyxml2-10.0.0)

create_single_source_cgal_program( "xyz2tin.cpp" )

#target_link_libraries(main PUBLIC tinyxml2)