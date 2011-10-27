/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <QStringList>
#include <QApplication>

#include "mainwindow.h"
#include "translationloader.h"
#include "metatyperegistrator.h"

int main(int argc, char * argv[]) {
#ifdef Q_WS_X11
    QApplication::setGraphicsSystem("raster");
#endif

    QApplication app(argc, argv);

#ifdef Q_WS_MAC
    app.setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    MetaTypeRegistrator registrator;

    TranslationLoader loader;
    loader.load();
    app.installTranslator(loader.systemTranslator());
    app.installTranslator(loader.applicationTranslator());

    app.setOrganizationName("openocr.org");
    app.setApplicationName("Quneiform OCR");
    app.setApplicationVersion("0.0.1");
    MainWindow w;
    w.show();

    if(argc > 1) {
        bool do_recognize = false;
        QStringList files;
        QStringList params = QCoreApplication::arguments();
        params.removeFirst();
        foreach(QString p, params) {
            if(p == "-r" || p == "--recognize")
                do_recognize = true;
            else
                files.append(p);
        }

        if(!files.isEmpty()) {
            w.open(files);

            if(do_recognize)
                w.recognizeAll();
        }
    }

    return app.exec();
}
