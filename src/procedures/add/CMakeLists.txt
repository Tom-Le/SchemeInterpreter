ADD_LIBRARY(procedure-add MODULE procedure-add.c)
SET_TARGET_PROPERTIES(procedure-add
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-add scheme)

INSTALL(TARGETS procedure-add DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
