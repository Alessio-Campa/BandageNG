#pragma once

#include <QColor>
#include <QDialog>
#include <QHash>
#include <QString>

#include <string>
#include <vector>

class QTableWidget;
class TagValueNodeColorer;

class TagValueColoursDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagValueColoursDialog(TagValueNodeColorer *colorer, QWidget *parent = nullptr);

private slots:
    void resetToAutomatic();
    void accept() override;

private:
    TagValueNodeColorer *m_colorer;
    QString m_tagName;
    std::vector<std::string> m_values;
    QTableWidget *m_table;
    QHash<QString, QColor> m_overrides;
};
