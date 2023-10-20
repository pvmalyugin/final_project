TARGET = xlsx
CONFIG(debug, debug|release) {
    TARGET = xlsxd
}
TEMPLATE = lib
CONFIG   += plugin
CONFIG   += c++11
CONFIG   += build_xlsx_lib
DEFINES  += QT_BUILD_XLSX_LIB


win32 {
    !contains(QMAKE_TARGET.arch, x86_64) {
        message(" XlsxWriter for win32")
        DESTDIR = ../../lib/win32
        CONFIG(debug, debug|release) {
            DLLDESTDIR =../../debug/win32
        } else {
            DLLDESTDIR =../../release/win32
        }
        LIBS += -L"../../lib/win32"
    } else {
        message(" XlsxWriter for win64")
        DESTDIR = ../../lib/win64
        CONFIG(debug, debug|release) {
            DLLDESTDIR =../../debug/win64
        } else {
            DLLDESTDIR =../../release/win64
        }
        LIBS += -L"../../lib/win64"
    }

}

linux-*{
    message(" XlsxWriter for linux")
    CONFIG(debug, debug|release) {
        DESTDIR = ../../debug/linux
        LIBS += -L"../../debug/linux"
    } else {
        DESTDIR = ../../release/linux
        LIBS += -L"../../release/linux"
    }
}

macx {
    message(" XlsxWriter for MacOS")
    CONFIG(debug, debug|release) {
        DESTDIR = ../../debug/mac/TecScheme15.app/Contents/MacOS
        LIBS += -L"../../debug/mac/TecScheme15.app/Contents/MacOS"
    } else {
        DESTDIR = ../../release/mac/TecScheme15.app/Contents/MacOS
        LIBS += -L"../../release/mac/TecScheme15.app/Contents/MacOS"
    }
}


include(qtxlsx.pri)

