ADD_LIBRARY(procedure-lessequal MODULE procedure-lessequal.c)
SET_TARGET_PROPERTIES(procedure-lessequal
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-lessequal scheme)

INSTALL(TARGETS procedure-lessequal DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
