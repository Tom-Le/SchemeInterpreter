ADD_LIBRARY(procedure-multiply MODULE procedure-multiply.c)
SET_TARGET_PROPERTIES(procedure-multiply
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-multiply scheme)

INSTALL(TARGETS procedure-multiply DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
