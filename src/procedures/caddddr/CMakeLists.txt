ADD_LIBRARY(procedure-caddddr MODULE procedure-caddddr.c)
SET_TARGET_PROPERTIES(procedure-caddddr
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-caddddr scheme)

INSTALL(TARGETS procedure-caddddr DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
