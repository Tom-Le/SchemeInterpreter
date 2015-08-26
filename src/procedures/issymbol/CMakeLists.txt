ADD_LIBRARY(procedure-issymbol MODULE procedure-issymbol.c)
SET_TARGET_PROPERTIES(procedure-issymbol
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-issymbol scheme)

INSTALL(TARGETS procedure-issymbol DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
