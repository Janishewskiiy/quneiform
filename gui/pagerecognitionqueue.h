/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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


#ifndef PAGERECOGNITIONQUEUE_H
#define PAGERECOGNITIONQUEUE_H

#include <QThread>
#include <QQueue>
#include <QMutex>

class Document;
class Page;

class PageRecognitionQueue : public QThread
{
    Q_OBJECT
public:
    explicit PageRecognitionQueue(QObject *parent = 0);
    void add(Document * doc);
    void add(Page * p);
    bool isEmpty() const;
    void run();
    void setLanguage(int language);
private:
    QQueue<Page*> pages_;
    QMutex mutex_;
    int language_;
};

#endif // PAGERECOGNITIONQUEUE_H
