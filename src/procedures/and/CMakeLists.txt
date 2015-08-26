ADD_LIBRARY(procedure-and MODULE procedure-and.c)
SET_TARGET_PROPERTIES(procedure-and
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-and scheme)

INSTALL(TARGETS procedure-and DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
