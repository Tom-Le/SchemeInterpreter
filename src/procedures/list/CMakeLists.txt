ADD_LIBRARY(procedure-list MODULE procedure-list.c)
SET_TARGET_PROPERTIES(procedure-list
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-list scheme)

INSTALL(TARGETS procedure-list DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
