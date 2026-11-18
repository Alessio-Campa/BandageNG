//Copyright 2017 Ryan Wick

//This file is part of Bandage

//Bandage is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Bandage is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Bandage.  If not, see <http://www.gnu.org/licenses/>.


#include "memory.h"

#include <QDir>
#include <QSettings>

Memory::Memory()
{
    rememberedPath = QDir::homePath();

    commandLineCommand = NO_COMMAND;

    pathDialogIsVisible = false;
    queryPathDialogIsVisible = false;

    userSpecifiedPath = Path();
    userSpecifiedPathString = "";
    userSpecifiedPathCircular = false;

    terminalWidth = 80;
}

void Memory::loadRememberedPath()
{
    QSettings settings("BandageNG", "BandageNG");
    const QString path = settings.value("lastDirectory").toString();
    if (!path.isEmpty() && QDir(path).exists())
        rememberedPath = path;
}

void Memory::saveRememberedPath() const
{
    QSettings settings("BandageNG", "BandageNG");
    settings.setValue("lastDirectory", rememberedPath);
}



//This function clears all memory that is particular to a graph.  It should be
//called whenever a new graph is loaded.
void Memory::clearGraphSpecificMemory()
{
    userSpecifiedPath = Path();
    userSpecifiedPathString = "";
    userSpecifiedPathCircular = false;
}

