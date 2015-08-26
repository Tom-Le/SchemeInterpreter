ADD_LIBRARY(procedure-last MODULE procedure-last.c)
SET_TARGET_PROPERTIES(procedure-last
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-last scheme)

INSTALL(TARGETS procedure-last DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
