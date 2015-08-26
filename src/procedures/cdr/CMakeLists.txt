ADD_LIBRARY(procedure-cdr MODULE procedure-cdr.c)
SET_TARGET_PROPERTIES(procedure-cdr
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-cdr scheme)

INSTALL(TARGETS procedure-cdr DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
