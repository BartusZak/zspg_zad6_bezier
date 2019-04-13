HEADERS += \
    window.h \
    program.h \
    glfunctions.h \
    bezier.h
SOURCES += \
    main.cpp \
    window.cpp \
    program.cpp \
    bezier.cpp

OTHER_FILES += \
    data/Shader.fragment.glsl \
    data/Shader.vertex.glsl \
    data/Shader.geometry.glsl

win32{
    copydata.commands = $(COPY_DIR) "$$shell_path("$$PWD/data")" "$$shell_path("$$OUT_PWD")"
}

unix{
    copydata.commands = $(COPY_DIR) $$PWD/data/* $$OUT_PWD
}

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
