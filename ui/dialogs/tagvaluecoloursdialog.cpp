#include "tagvaluecoloursdialog.h"

#include "graph/nodecolorers.h"
#include "program/globals.h"
#include "program/settings.h"
#include "ui/widgets/colourbutton.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

TagValueColoursDialog::TagValueColoursDialog(TagValueNodeColorer *colorer, QWidget *parent) :
    QDialog(parent), m_colorer(colorer)
{
    setWindowTitle("GFA tag colours");

    m_tagName = QString::fromStdString(m_colorer->tagName());
    m_values = m_colorer->valuesForTag(m_colorer->tagName());
    m_overrides = g_settings->tagValueColours;

    auto *layout = new QVBoxLayout(this);

    auto *description = new QLabel(
            QString("Choose the colour for each value of the '%1' tag. Colours that are valid "
                    "hex values are used automatically, but can be overridden here.").arg(m_tagName),
            this);
    description->setWordWrap(true);
    layout->addWidget(description);

    m_table = new QTableWidget(int(m_values.size()), 2, this);
    m_table->setHorizontalHeaderLabels({"Value", "Colour"});
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int row = 0; row < int(m_values.size()); ++row) {
        const QString value = QString::fromStdString(m_values[row]);
        auto *valueItem = new QTableWidgetItem(value);
        valueItem->setToolTip(value);
        m_table->setItem(row, 0, valueItem);

        auto *colourButton = new ColourButton(m_table);
        colourButton->m_name = "Tag value colour";
        colourButton->setColour(m_colorer->effectiveColour(m_colorer->tagName(), m_values[row]));
        m_table->setCellWidget(row, 1, colourButton);
    }

    layout->addWidget(m_table, 1);

    auto *bottomLayout = new QHBoxLayout();
    auto *resetButton = new QPushButton("Reset all to automatic", this);
    connect(resetButton, &QPushButton::clicked, this, &TagValueColoursDialog::resetToAutomatic);
    bottomLayout->addWidget(resetButton);
    bottomLayout->addStretch();

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &TagValueColoursDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    bottomLayout->addWidget(buttons);

    layout->addLayout(bottomLayout);

    resize(700, 450);
}

void TagValueColoursDialog::resetToAutomatic()
{
    for (int row = 0; row < int(m_values.size()); ++row) {
        if (auto *button = qobject_cast<ColourButton *>(m_table->cellWidget(row, 1)))
            button->setColour(m_colorer->automaticColour(m_colorer->tagName(), m_values[row]));
    }
}

void TagValueColoursDialog::accept()
{
    for (int row = 0; row < int(m_values.size()); ++row) {
        auto *button = qobject_cast<ColourButton *>(m_table->cellWidget(row, 1));
        if (!button)
            continue;

        const QString tagName = QString::fromStdString(m_colorer->tagName());
        const QString value = QString::fromStdString(m_values[row]);
        const QString key = Settings::tagValueColourKey(tagName, value);
        const QColor automatic = m_colorer->automaticColour(m_colorer->tagName(), m_values[row]);

        if (button->m_colour == automatic)
            m_overrides.remove(key);
        else
            m_overrides.insert(key, button->m_colour);
    }

    g_settings->tagValueColours = m_overrides;
    QDialog::accept();
}
