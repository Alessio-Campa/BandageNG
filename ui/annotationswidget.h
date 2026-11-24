#ifndef BANDAGE_ANNOTATIONSWIDGET_H
#define BANDAGE_ANNOTATIONSWIDGET_H


#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDialog>
#include <QFormLayout>
#include <QCheckBox>

#include "graph/annotationsmanager.h"


class AnnotationSettingsDialog : public QDialog {
Q_OBJECT

public:
    explicit AnnotationSettingsDialog(const AnnotationGroup &annotationGroup, QWidget *parent = nullptr);

private:
    AnnotationGroupId m_annotationGroupId;
};


class AnnotationsListWidget : public QListWidget {
Q_OBJECT

public:
    explicit AnnotationsListWidget(QWidget *parent);

    void setManager(AnnotationsManager * manager);

public slots:
    void updateAnnotationGroups();

private:
    AnnotationsManager * m_manager = nullptr;
};


#endif //BANDAGE_ANNOTATIONSWIDGET_H
