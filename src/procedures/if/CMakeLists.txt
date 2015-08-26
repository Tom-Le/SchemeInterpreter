ADD_LIBRARY(procedure-if MODULE procedure-if.c)
SET_TARGET_PROPERTIES(procedure-if
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-if scheme)

INSTALL(TARGETS procedure-if DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
