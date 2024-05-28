file(GLOB_RECURSE
	LANGULUS_MOD_INPUTSDL_DEMO_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp
)

add_executable(LangulusModInputSDLDemo ${LANGULUS_MOD_INPUTSDL_DEMO_SOURCES})

target_link_libraries(LangulusModInputSDLDemo
	PRIVATE		Langulus
)

add_dependencies(LangulusModInputSDLDemo
	LangulusModInputSDL
)
