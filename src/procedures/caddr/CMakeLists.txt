ADD_LIBRARY(procedure-caddr MODULE procedure-caddr.c)
SET_TARGET_PROPERTIES(procedure-caddr
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-caddr scheme)

INSTALL(TARGETS procedure-caddr DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
