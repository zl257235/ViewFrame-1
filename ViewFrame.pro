TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    Base \
    Network \
    Core

vari = getenv(CONFIG)
message($$vari)

TRANSLATIONS += $${TARGET}_zh_CN.ts
TRANSLATIONS += $${TARGET}_en_US.ts
