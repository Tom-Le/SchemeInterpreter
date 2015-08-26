ADD_LIBRARY(procedure-append MODULE procedure-append.c)
SET_TARGET_PROPERTIES(procedure-append
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-append scheme)

INSTALL(TARGETS procedure-append DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
