ADD_LIBRARY(procedure-greater MODULE procedure-greater.c)
SET_TARGET_PROPERTIES(procedure-greater
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-greater scheme)

INSTALL(TARGETS procedure-greater DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
