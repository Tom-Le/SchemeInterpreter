ADD_EXECUTABLE(scheme main.c)
TARGET_LINK_LIBRARIES(scheme scheme_modules scheme_types scheme_procedures)

INSTALL(TARGETS scheme DESTINATION ${CMAKE_BINARY_DIR}/bin)
