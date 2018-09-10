TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    Base \
    Network \
    Core

#for ( sub,SUBDIRS){
#    message($$sub)
#}

vari = getenv(CONFIG)
message($$vari)
