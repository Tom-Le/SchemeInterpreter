ADD_LIBRARY(procedure-define MODULE procedure-define.c)
SET_TARGET_PROPERTIES(procedure-define
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-define scheme)

INSTALL(TARGETS procedure-define DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
