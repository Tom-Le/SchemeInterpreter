ADD_LIBRARY(procedure-exit MODULE procedure-exit.c)
SET_TARGET_PROPERTIES(procedure-exit
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-exit scheme)

INSTALL(TARGETS procedure-exit DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
