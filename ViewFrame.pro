TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    Util \
    Network \
    Base \
    Core

#for ( sub,SUBDIRS){
#    message($$sub)
#}

vari = getenv(CONFIG)
message($$vari)
