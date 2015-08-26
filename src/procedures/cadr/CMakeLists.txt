ADD_LIBRARY(procedure-cadr MODULE procedure-cadr.c)
SET_TARGET_PROPERTIES(procedure-cadr
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-cadr scheme)

INSTALL(TARGETS procedure-cadr DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
