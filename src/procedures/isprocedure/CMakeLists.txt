ADD_LIBRARY(procedure-isprocedure MODULE procedure-isprocedure.c)
SET_TARGET_PROPERTIES(procedure-isprocedure
                      PROPERTIES POSITION_INDEPENDENT_CODE ON
                                 PREFIX ""
                                 SUFFIX ".so")
TARGET_LINK_LIBRARIES(procedure-isprocedure scheme)

INSTALL(TARGETS procedure-isprocedure DESTINATION ${PROCEDURE_INSTALL_DESTINATION})
