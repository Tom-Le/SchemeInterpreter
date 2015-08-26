ADD_LIBRARY(procedure-let MODULE procedure-let.c)
SET_TARGET_PROPERTIES(procedure-let
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-let scheme)

INSTALL(TARGETS procedure-let DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
