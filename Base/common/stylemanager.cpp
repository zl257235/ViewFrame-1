#include "stylemanager.h"

#include <QFile>
#include <QApplication>

#include "../util/rlog.h"

namespace Base{

class StyleManagerPrivate
{
    Q_DECLARE_PUBLIC(StyleManager)

public:
    StyleManagerPrivate(StyleManager * q):q_ptr(q){

    }

    StyleManager * q_ptr;
    StyleList slist;
};

StyleManager::StyleManager():d_ptr(new StyleManagerPrivate(this)),QObject()
{

}

void StyleManager::addStyle(CustomStyle &style)
{
    Q_D(StyleManager);
    d->slist.push_back(style);
}

int StyleManager::size()
{
    Q_D(StyleManager);
    return d->slist.size();
}

StyleList StyleManager::styles()
{
    Q_D(StyleManager);
    return d->slist;
}

void StyleManager::switchStyle(int index)
{
    Q_D(StyleManager);
    Q_ASSERT(index < d->slist.size());
    CustomStyle style = d->slist.at(index);
    switch(style.getStyleType()){
        case CustomStyle::STYLE_SHEET:
            {
                QFile styleFile(style.getStylePath());
                if(!styleFile.open(QFile::ReadOnly)){
                    RLOG_ERROR("style file %s read error!",style.getStylePath().toLocal8Bit().data());
                    return;
                }
                qApp->setStyleSheet(styleFile.readAll());
            }
            break;
        case CustomStyle::STYLE_CUSTOMSTYLE:
            switch(style.getClazz()){
                //qApp->setStyle();
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

}
