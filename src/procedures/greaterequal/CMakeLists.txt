ADD_LIBRARY(procedure-greaterequal MODULE procedure-greaterequal.c)
SET_TARGET_PROPERTIES(procedure-greaterequal
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-greaterequal scheme)

INSTALL(TARGETS procedure-greaterequal DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
