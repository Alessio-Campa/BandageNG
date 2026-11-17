#pragma once

#include "program/settings.h"

#include <QDialog>
#include <QStringList>

class ColourButton;
class QComboBox;
class QTableWidget;

class ConditionalColoursDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConditionalColoursDialog(QWidget *parent = nullptr);

private slots:
    void addRule();
    void removeRule();
    void moveRuleUp();
    void moveRuleDown();
    void accept() override;

private:
    void addRuleRow(int row, const TagColourRule &rule);
    [[nodiscard]] TagColourRule ruleAt(int row) const;
    [[nodiscard]] QStringList availableTagNames() const;

    QTableWidget *m_table;
    ColourButton *m_defaultColourButton = nullptr;
    QStringList m_tagNames;
};
