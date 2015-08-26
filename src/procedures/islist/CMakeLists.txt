ADD_LIBRARY(procedure-islist MODULE procedure-islist.c)
SET_TARGET_PROPERTIES(procedure-islist
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-islist scheme)

INSTALL(TARGETS procedure-islist DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
