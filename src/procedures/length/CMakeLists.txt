ADD_LIBRARY(procedure-length MODULE procedure-length.c)
SET_TARGET_PROPERTIES(procedure-length
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-length scheme)

INSTALL(TARGETS procedure-length DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
