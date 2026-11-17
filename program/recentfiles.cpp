#include "recentfiles.h"

#include <QFileInfo>
#include <QSettings>

namespace {

constexpr auto RecentFilesKey = "recentFiles";
constexpr auto RecentFilesLimitKey = "recentFilesLimit";

QSettings makeSettings()
{
    return QSettings("BandageNG", "BandageNG");
}

int clampedLimit(int value)
{
    return qBound(recentfiles::DefaultLimit, value, recentfiles::MaximumLimit);
}

}

namespace recentfiles {

int limit()
{
    QSettings settings = makeSettings();
    return clampedLimit(settings.value(RecentFilesLimitKey, DefaultLimit).toInt());
}

void setLimit(int newLimit)
{
    newLimit = clampedLimit(newLimit);

    QSettings settings = makeSettings();
    settings.setValue(RecentFilesLimitKey, newLimit);

    QStringList files = settings.value(RecentFilesKey).toStringList();
    while (files.size() > newLimit)
        files.removeLast();
    settings.setValue(RecentFilesKey, files);
}

QStringList existingPaths()
{
    QSettings settings = makeSettings();
    const QStringList stored = settings.value(RecentFilesKey).toStringList();

    QStringList existing;
    existing.reserve(stored.size());
    for (const QString &path : stored) {
        if (!path.isEmpty() && QFileInfo::exists(path))
            existing.append(path);
    }

    while (existing.size() > limit())
        existing.removeLast();

    if (existing != stored)
        settings.setValue(RecentFilesKey, existing);

    return existing;
}

void addPath(const QString &fullFileName)
{
    if (fullFileName.isEmpty())
        return;

    const QString path = QFileInfo(fullFileName).absoluteFilePath();

    QSettings settings = makeSettings();
    QStringList files = settings.value(RecentFilesKey).toStringList();
    files.removeAll(path);
    files.prepend(path);
    while (files.size() > limit())
        files.removeLast();

    settings.setValue(RecentFilesKey, files);
}

void clear()
{
    QSettings settings = makeSettings();
    settings.remove(RecentFilesKey);
}

}
