#include "conditionalcoloursdialog.h"

#include "graph/assemblygraph.h"
#include "program/globals.h"
#include "program/settings.h"
#include "ui/widgets/colourbutton.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSet>
#include <QTableWidget>
#include <QVBoxLayout>

ConditionalColoursDialog::ConditionalColoursDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Conditional colours");

    m_tagNames = availableTagNames();

    auto *layout = new QVBoxLayout(this);

    auto *description = new QLabel(
            "Rules are checked from top to bottom and the first matching rule wins. "
            "Matching is case-insensitive. New rule colours start from the tag colour palette "
            "and can be changed freely. Nodes that do not match any rule use the default colour.",
            this);
    description->setWordWrap(true);
    layout->addWidget(description);

    m_table = new QTableWidget(0, 4, this);
    m_table->setHorizontalHeaderLabels({"Tag", "Match", "Value", "Colour"});
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    m_table->setColumnWidth(0, 110);
    m_table->setColumnWidth(1, 150);
    m_table->setColumnWidth(3, 120);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(32);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);

    for (const auto &rule : g_settings->tagColourRules)
        addRuleRow(m_table->rowCount(), rule);

    layout->addWidget(m_table, 1);

    auto *ruleButtons = new QHBoxLayout();
    auto *addButton = new QPushButton("Add rule", this);
    auto *removeButton = new QPushButton("Remove selected", this);
    auto *upButton = new QPushButton("Move up", this);
    auto *downButton = new QPushButton("Move down", this);
    ruleButtons->addWidget(addButton);
    ruleButtons->addWidget(removeButton);
    ruleButtons->addWidget(upButton);
    ruleButtons->addWidget(downButton);
    ruleButtons->addStretch();
    ruleButtons->addWidget(new QLabel("Default colour:", this));

    m_defaultColourButton = new ColourButton(this);
    m_defaultColourButton->m_name = "Default node colour";
    m_defaultColourButton->setColour(g_settings->conditionalColour);
    ruleButtons->addWidget(m_defaultColourButton);

    layout->addLayout(ruleButtons);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &ConditionalColoursDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    connect(addButton, &QPushButton::clicked, this, &ConditionalColoursDialog::addRule);
    connect(removeButton, &QPushButton::clicked, this, &ConditionalColoursDialog::removeRule);
    connect(upButton, &QPushButton::clicked, this, &ConditionalColoursDialog::moveRuleUp);
    connect(downButton, &QPushButton::clicked, this, &ConditionalColoursDialog::moveRuleDown);

    resize(800, 450);
}

QStringList ConditionalColoursDialog::availableTagNames() const
{
    QSet<QString> names;

    for (const auto &rule : g_settings->tagColourRules) {
        if (!rule.tagName.isEmpty())
            names.insert(rule.tagName);
    }

    for (const auto &entry : g_assemblyGraph->m_nodeTags) {
        for (const auto &tag : entry.second)
            names.insert(QString::fromLatin1(tag.name, 2));
    }

    QStringList result(names.begin(), names.end());
    result.sort(Qt::CaseInsensitive);
    return result;
}

void ConditionalColoursDialog::addRuleRow(int row, const TagColourRule &rule)
{
    m_table->insertRow(row);
    m_table->setRowHeight(row, 32);

    auto *tagCombo = new QComboBox(m_table);
    tagCombo->setEditable(true);
    tagCombo->setInsertPolicy(QComboBox::NoInsert);
    tagCombo->addItems(m_tagNames);
    tagCombo->setCurrentText(rule.tagName);
    tagCombo->setMinimumWidth(90);
    m_table->setCellWidget(row, 0, tagCombo);

    auto *matchCombo = new QComboBox(m_table);
    matchCombo->addItem("equals", int(TagMatchType::Equals));
    matchCombo->addItem("contains", int(TagMatchType::Contains));
    matchCombo->addItem("csv includes", int(TagMatchType::CsvIncludes));
    matchCombo->addItem("starts with", int(TagMatchType::StartsWith));
    matchCombo->addItem("ends with", int(TagMatchType::EndsWith));
    const int matchIndex = matchCombo->findData(int(rule.matchType));
    matchCombo->setCurrentIndex(matchIndex >= 0 ? matchIndex : 0);
    matchCombo->setMinimumWidth(130);
    m_table->setCellWidget(row, 1, matchCombo);

    auto *valueEdit = new QLineEdit(rule.value, m_table);
    m_table->setCellWidget(row, 2, valueEdit);

    QColor colour = rule.colour;
    if (!colour.isValid()) {
        QVector<QColor> palette = g_settings->tagColourPalette;
        if (palette.isEmpty())
            palette = Settings::defaultTagColourPalette();
        colour = palette.isEmpty() ? g_settings->conditionalColour
                                   : palette[row % palette.size()];
    }

    auto *colourButton = new ColourButton(m_table);
    colourButton->m_name = "Rule colour";
    colourButton->setColour(colour);
    m_table->setCellWidget(row, 3, colourButton);
}

TagColourRule ConditionalColoursDialog::ruleAt(int row) const
{
    TagColourRule rule;

    if (auto *tagCombo = qobject_cast<QComboBox *>(m_table->cellWidget(row, 0)))
        rule.tagName = tagCombo->currentText().trimmed();

    if (auto *matchCombo = qobject_cast<QComboBox *>(m_table->cellWidget(row, 1)))
        rule.matchType = TagMatchType(matchCombo->currentData().toInt());

    if (auto *valueEdit = qobject_cast<QLineEdit *>(m_table->cellWidget(row, 2)))
        rule.value = valueEdit->text();

    if (auto *colourButton = qobject_cast<ColourButton *>(m_table->cellWidget(row, 3)))
        rule.colour = colourButton->m_colour;

    return rule;
}

void ConditionalColoursDialog::addRule()
{
    QVector<QColor> palette = g_settings->tagColourPalette;
    if (palette.isEmpty())
        palette = Settings::defaultTagColourPalette();

    TagColourRule rule;
    rule.colour = palette.isEmpty()
            ? g_settings->conditionalColour
            : palette[m_table->rowCount() % palette.size()];
    addRuleRow(m_table->rowCount(), rule);
    m_table->setCurrentCell(m_table->rowCount() - 1, 0);
}

void ConditionalColoursDialog::removeRule()
{
    const int row = m_table->currentRow();
    if (row >= 0)
        m_table->removeRow(row);
}

void ConditionalColoursDialog::moveRuleUp()
{
    const int row = m_table->currentRow();
    if (row <= 0)
        return;

    const TagColourRule rule = ruleAt(row);
    m_table->removeRow(row);
    addRuleRow(row - 1, rule);
    m_table->setCurrentCell(row - 1, 0);
}

void ConditionalColoursDialog::moveRuleDown()
{
    const int row = m_table->currentRow();
    if (row < 0 || row >= m_table->rowCount() - 1)
        return;

    const TagColourRule rule = ruleAt(row);
    m_table->removeRow(row);
    addRuleRow(row + 1, rule);
    m_table->setCurrentCell(row + 1, 0);
}

void ConditionalColoursDialog::accept()
{
    QVector<TagColourRule> rules;
    rules.reserve(m_table->rowCount());

    for (int row = 0; row < m_table->rowCount(); ++row) {
        const TagColourRule rule = ruleAt(row);
        if (!rule.tagName.isEmpty())
            rules.append(rule);
    }

    g_settings->tagColourRules = rules;
    g_settings->conditionalColour = m_defaultColourButton->m_colour;

    QDialog::accept();
}
