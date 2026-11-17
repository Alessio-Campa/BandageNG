#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QString>
#include <QStringList>

namespace recentfiles {

constexpr int DefaultLimit = 5;
constexpr int MaximumLimit = 10;

//Returns the stored paths, most recent first. Missing files are removed.
QStringList existingPaths();

int limit();
void setLimit(int limit);

void addPath(const QString &fullFileName);
void clear();

}

#endif // RECENTFILES_H
