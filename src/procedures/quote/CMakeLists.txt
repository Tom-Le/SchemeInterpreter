ADD_LIBRARY(procedure-quote MODULE procedure-quote.c)
SET_TARGET_PROPERTIES(procedure-quote
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-quote scheme)

INSTALL(TARGETS procedure-quote DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
