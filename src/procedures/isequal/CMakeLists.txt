ADD_LIBRARY(procedure-isequal MODULE procedure-isequal.c)
SET_TARGET_PROPERTIES(procedure-isequal
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-isequal scheme)

INSTALL(TARGETS procedure-isequal DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
