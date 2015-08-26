ADD_LIBRARY(procedure-cons MODULE procedure-cons.c)
SET_TARGET_PROPERTIES(procedure-cons
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-cons scheme)

INSTALL(TARGETS procedure-cons DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
