/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <QDebug>
#include "dummyspellchecker.h"

DummySpellChecker::DummySpellChecker(const Language& lang) : ISpellChecker(lang)
{
    qDebug() << Q_FUNC_INFO << "Using dummy spellchecker for language:" << lang.name();
}

bool DummySpellChecker::hasErrors(const QString&) {
    return false;
}

ISpellChecker::SpellList DummySpellChecker::spellErrors(const QString&) {
    return ISpellChecker::SpellList();
}

QStringList DummySpellChecker::suggest(const QString&) {
    return QStringList();
}

QList<Language> DummySpellChecker::supportedLanguages() const {
    return QList<Language>();
}
