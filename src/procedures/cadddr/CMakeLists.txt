ADD_LIBRARY(procedure-cadddr MODULE procedure-cadddr.c)
SET_TARGET_PROPERTIES(procedure-cadddr
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-cadddr scheme)

INSTALL(TARGETS procedure-cadddr DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
