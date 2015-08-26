ADD_LIBRARY(procedure-less MODULE procedure-less.c)
SET_TARGET_PROPERTIES(procedure-less
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-less scheme)

INSTALL(TARGETS procedure-less DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
