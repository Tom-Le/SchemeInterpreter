ADD_LIBRARY(procedure-subtract MODULE procedure-subtract.c)
SET_TARGET_PROPERTIES(procedure-subtract
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-subtract scheme)

INSTALL(TARGETS procedure-subtract DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
