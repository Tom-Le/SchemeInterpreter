ADD_LIBRARY(procedure-cond MODULE procedure-cond.c)
SET_TARGET_PROPERTIES(procedure-cond
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-cond scheme)

INSTALL(TARGETS procedure-cond DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
