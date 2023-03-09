set(INCLUDES 
)
set(SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
)
set(LIBS
nimo_engine
nfd
)

add_executable(nimo_editor ${SOURCES})
set_target_properties(nimo_editor PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(nimo_editor PUBLIC ${INCLUDES})
target_link_libraries(nimo_editor PUBLIC ${LIBS})