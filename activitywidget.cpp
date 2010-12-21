#include "activitywidget.h"

#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/LineEdit>
#include <Plasma/PushButton>

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>

ActivityWidget::ActivityWidget(QGraphicsItem *parent, QString id) : QGraphicsWidget(parent), m_layout(0), m_removeWidget(0), m_editWidget(0), m_label(0), m_stateIcon(0), m_addIcon(0), m_removeIcon(0), m_id(id), m_dialogShown(false) {
  m_layout = new QGraphicsGridLayout(this);
  m_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(m_layout);
  // create label
  m_label = new Plasma::IconWidget(this);
  m_label->setOrientation(Qt::Horizontal);
  m_label->setPreferredSize(32, 32);
  m_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_layout->addItem(m_label, 0, 0, Qt::AlignCenter);
  // setCurrent the activity when clicked on the name
  connect(m_label, SIGNAL(clicked()), this, SLOT(setCurrent()));
  // create status icon
  m_stateIcon = new Plasma::IconWidget(this);
  m_stateIcon->setOrientation(Qt::Horizontal);
  m_stateIcon->setIcon("media-playback-start");
  m_stateIcon->setPreferredSize(22, 22);
  m_stateIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_layout->addItem(m_stateIcon, 0, 1, Qt::AlignCenter);
  // toggle running state when clicked on the icon
  connect(m_stateIcon, SIGNAL(clicked()), this, SLOT(toggleStatus()));
  // create add icon
  m_addIcon = new Plasma::IconWidget(this);
  m_addIcon->setOrientation(Qt::Horizontal);
  m_addIcon->setSvg("widgets/action-overlays", "add-normal");
  m_addIcon->setPreferredSize(22, 22);
  m_addIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_layout->addItem(m_addIcon, 0, 2, Qt::AlignCenter);
  // begin adding
  connect(m_addIcon, SIGNAL(clicked()), this, SLOT(beginAdd()));
  // create edit icon
  m_editIcon = new Plasma::IconWidget(this);
  m_editIcon->setOrientation(Qt::Horizontal);
  m_editIcon->setSvg("widgets/configuration-icons", "configure");
  m_editIcon->setPreferredSize(16, 16);
  m_editIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_layout->addItem(m_editIcon, 0, 3, Qt::AlignCenter);
  // begin editing
  connect(m_editIcon, SIGNAL(clicked()), this, SLOT(beginEdit()));
  // create remove icon
  m_removeIcon = new Plasma::IconWidget(this);
  m_removeIcon->setOrientation(Qt::Horizontal);
  m_removeIcon->setSvg("widgets/configuration-icons", "close");
  m_removeIcon->setPreferredSize(16, 16);
  m_removeIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_layout->addItem(m_removeIcon, 0, 4, Qt::AlignCenter);
  // ask for confirmation
  connect(m_removeIcon, SIGNAL(clicked()), this, SLOT(beginRemove()));
}

QString ActivityWidget::name() {
  return m_name;
}

void ActivityWidget::setName(QString name) {
  m_name = name;
  // update label
  m_label->setText(m_name);
}

void ActivityWidget::setIcon(QString icon) {
  // update label
  if (icon != "")
    m_label->setIcon(icon);
  else
    m_label->setIcon("plasma");
}

void ActivityWidget::setCurrent(bool current) {
  // update label
  QFont font = m_label->font();
  if (current)
    font.setWeight(QFont::Bold);
  else
    font.setWeight(QFont::Normal);
  m_label->setFont(font);
}

void ActivityWidget::setState(QString state) {
  m_state = state;
  // update icon
  if (m_state == "Stopped")
    m_stateIcon->setIcon("media-playback-start");
  else
    m_stateIcon->setIcon("media-playback-stop");
}

void ActivityWidget::lock() {
  m_addIcon->setVisible(false);
  m_editIcon->setVisible(false);
  m_removeIcon->setVisible(false);
}

void ActivityWidget::unlock() {
  m_addIcon->setVisible(true);
  m_editIcon->setVisible(true);
  m_removeIcon->setVisible(true);
}

void ActivityWidget::setCurrent() {
  emit setCurrent(m_id);
}

void ActivityWidget::toggleStatus() {
  if (m_state == "Stopped")
    emit startActivity(m_id);
  else
    emit stopActivity(m_id);
}

void ActivityWidget::beginRemove() {
  if (m_dialogShown)
    return;
  // turn on the removing flag
  m_dialogShown = true;
  // create confirmation widget
  m_removeWidget = new QGraphicsWidget();
  // create a horizontal layout
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(m_removeWidget);
  layout->setOrientation(Qt::Horizontal);
  // set the layout
  m_removeWidget->setLayout(layout);
  // create label
  Plasma::Label *label = new Plasma::Label(m_removeWidget);
  label->setText(i18n("Do you really want to remove this activity?"));
  layout->addItem(label);
  // create yes button
  Plasma::PushButton *yesButton = new Plasma::PushButton(m_removeWidget);
  yesButton->setText(i18n("Yes"));
  layout->addItem(yesButton);
  connect(yesButton, SIGNAL(clicked()), this, SLOT(acceptRemove()));
  // create no button
  Plasma::PushButton *noButton = new Plasma::PushButton(m_removeWidget);
  noButton->setText(i18n("No"));
  layout->addItem(noButton);
  connect(noButton, SIGNAL(clicked()), this, SLOT(cancelRemove()));
  // show the confirm widget
  m_layout->addItem(m_removeWidget, 1, 0, 1, 5);
}

void ActivityWidget::acceptRemove() {
  // delete the confirm widget
  m_removeWidget->deleteLater();
  // emit delete signal
  emit removeActivity(m_id);
  // turn off the removing flag
  m_dialogShown = false;
}

void ActivityWidget::cancelRemove() {
  // delete the confirm widget
  m_removeWidget->deleteLater();
  // turn off the removing flag
  m_dialogShown = false;
}

void ActivityWidget::beginEdit() {
  if (m_dialogShown)
    return;
  // turn on the editing flag
  m_dialogShown = true;
  // create confirmation widget
  m_editWidget = new QGraphicsWidget();
  // create a horizontal layout
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(m_editWidget);
  layout->setOrientation(Qt::Horizontal);
  // set the layout
  m_editWidget->setLayout(layout);
  // create label
  Plasma::Label *label = new Plasma::Label(m_editWidget);
  label->setText(i18n("Name:"));
  layout->addItem(label);
  // create the line edit
  m_lineEdit = new Plasma::LineEdit(m_editWidget);
  m_lineEdit->setText(m_name);
  layout->addItem(m_lineEdit);
  // create yes button
  Plasma::PushButton *yesButton = new Plasma::PushButton(m_editWidget);
  yesButton->setText(i18n("OK"));
  layout->addItem(yesButton);
  connect(yesButton, SIGNAL(clicked()), this, SLOT(acceptEdit()));
  // create no button
  Plasma::PushButton *noButton = new Plasma::PushButton(m_editWidget);
  noButton->setText(i18n("Cancel"));
  layout->addItem(noButton);
  connect(noButton, SIGNAL(clicked()), this, SLOT(cancelEdit()));
  // show the confirm widget
  m_layout->addItem(m_editWidget, 1, 0, 1, 5);
  // put the focus into the line edit
  m_lineEdit->setFocus(Qt::OtherFocusReason);
}

void ActivityWidget::acceptEdit() {
  // save the name
  QString name = m_lineEdit->text();
  // delete the edit widget
  m_editWidget->deleteLater();
  // emit rename signal
  emit renameActivity(m_id, name);
  // turn of the editing flag
  m_dialogShown = false;
}

void ActivityWidget::cancelEdit() {
  // delete the edit widget
  m_editWidget->deleteLater();
  // turn off the editing flag
  m_dialogShown = false;
}

void ActivityWidget::beginAdd() {
  // emit add signal
  emit addActivity(m_id);
}
