ADD_LIBRARY(procedure-isnull MODULE procedure-isnull.c)
SET_TARGET_PROPERTIES(procedure-isnull
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-isnull scheme)

INSTALL(TARGETS procedure-isnull DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
