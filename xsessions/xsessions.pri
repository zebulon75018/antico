XSESSION_FILES = ../../xsessions/*.in

desktop_compiler.name = Desktop_Compiler files ${QMAKE_FILE_BASE}
desktop_compiler.CONFIG += no_link target_predeps
unix:desktop_compiler.commands = sed "s,@PREFIX@,$$PREFIX,g" < ${QMAKE_FILE_IN} > ${QMAKE_FILE_OUT}
desktop_compiler.input = XSESSION_FILES
desktop_compiler.output = ${QMAKE_FILE_BASE}
desktop_compiler.clean = ${QMAKE_FILE_BASE}
QMAKE_EXTRA_COMPILERS += desktop_compiler
