ADD_LIBRARY(procedure-or MODULE procedure-or.c)
SET_TARGET_PROPERTIES(procedure-or
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-or scheme)

INSTALL(TARGETS procedure-or DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
