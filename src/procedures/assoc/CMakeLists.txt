ADD_LIBRARY(procedure-assoc MODULE procedure-assoc.c)
SET_TARGET_PROPERTIES(procedure-assoc
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-assoc scheme)

INSTALL(TARGETS procedure-assoc DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
