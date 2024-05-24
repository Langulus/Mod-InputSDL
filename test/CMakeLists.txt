file(GLOB_RECURSE
	LANGULUS_MOD_INPUTSDL_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp
)

add_executable(LangulusModInputSDLTest ${LANGULUS_MOD_INPUTSDL_TEST_SOURCES})

target_link_libraries(LangulusModInputSDLTest
	PRIVATE		Langulus
				Catch2
)

add_dependencies(LangulusModInputSDLTest
	LangulusModInputSDL
)

add_test(
	NAME		LangulusModInputSDLTest
	COMMAND		LangulusModInputSDLTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)