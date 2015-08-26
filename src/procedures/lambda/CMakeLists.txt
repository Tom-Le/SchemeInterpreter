ADD_LIBRARY(procedure-lambda MODULE procedure-lambda.c)
SET_TARGET_PROPERTIES(procedure-lambda
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-lambda scheme)

INSTALL(TARGETS procedure-lambda DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
