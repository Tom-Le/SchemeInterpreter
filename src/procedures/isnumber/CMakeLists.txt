ADD_LIBRARY(procedure-isnumber MODULE procedure-isnumber.c)
SET_TARGET_PROPERTIES(procedure-isnumber
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-isnumber scheme)

INSTALL(TARGETS procedure-isnumber DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
