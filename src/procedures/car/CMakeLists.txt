ADD_LIBRARY(procedure-car MODULE procedure-car.c)
SET_TARGET_PROPERTIES(procedure-car
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-car scheme)

INSTALL(TARGETS procedure-car DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
