#include "activitywidget.h"

#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/LineEdit>
#include <Plasma/PushButton>

#include <QGraphicsLinearLayout>

ActivityWidget::ActivityWidget(QGraphicsItem *parent, QString id) : QGraphicsWidget(parent), m_layout(0), m_removeWidget(0), m_editWidget(0), m_label(0), m_stateIcon(0), m_addIcon(0), m_removeIcon(0), m_id(id) {
  m_layout = new QGraphicsLinearLayout(this);
  m_layout->setOrientation(Qt::Vertical);
  m_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(m_layout);
  // create activity layout
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(m_layout);
  layout->setOrientation(Qt::Horizontal);
  layout->setContentsMargins(0, 0, 0, 0);
  m_layout->addItem(layout);
  // create label
  m_label = new Plasma::IconWidget(this);
  m_label->setOrientation(Qt::Horizontal);
  m_label->setPreferredSize(32, 32);
  m_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  layout->addItem(m_label);
  layout->setAlignment(m_label, Qt::AlignCenter);
  // setCurrent the activity when clicked on the name
  connect(m_label, SIGNAL(clicked()), this, SLOT(setCurrent()));
  // create status icon
  m_stateIcon = new Plasma::IconWidget(this);
  m_stateIcon->setOrientation(Qt::Horizontal);
  m_stateIcon->setIcon("media-playback-start");
  m_stateIcon->setPreferredSize(22, 22);
  m_stateIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addItem(m_stateIcon);
  layout->setAlignment(m_stateIcon, Qt::AlignCenter);
  // toggle running state when clicked on the icon
  connect(m_stateIcon, SIGNAL(clicked()), this, SLOT(toggleStatus()));
  // create add icon
  m_addIcon = new Plasma::IconWidget(this);
  m_addIcon->setOrientation(Qt::Horizontal);
  m_addIcon->setSvg("widgets/action-overlays", "add-normal");
  m_addIcon->setPreferredSize(22, 22);
  m_addIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addItem(m_addIcon);
  layout->setAlignment(m_addIcon, Qt::AlignCenter);
  // begin adding
  connect(m_addIcon, SIGNAL(clicked()), this, SLOT(beginAdd()));
  // create edit icon
  m_editIcon = new Plasma::IconWidget(this);
  m_editIcon->setOrientation(Qt::Horizontal);
  m_editIcon->setSvg("widgets/configuration-icons", "configure");
  m_editIcon->setPreferredSize(16, 16);
  m_editIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addItem(m_editIcon);
  layout->setAlignment(m_editIcon, Qt::AlignCenter);
  // begin editing
  connect(m_editIcon, SIGNAL(clicked()), this, SLOT(beginEdit()));
  // create remove icon
  m_removeIcon = new Plasma::IconWidget(this);
  m_removeIcon->setOrientation(Qt::Horizontal);
  m_removeIcon->setSvg("widgets/configuration-icons", "close");
  m_removeIcon->setPreferredSize(16, 16);
  m_removeIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addItem(m_removeIcon);
  layout->setAlignment(m_removeIcon, Qt::AlignCenter);
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
  m_layout->addItem(m_removeWidget);
}

void ActivityWidget::acceptRemove() {
  // hide the confirm widget
  m_layout->removeItem(m_removeWidget);
  // delete the confirm widget
  m_removeWidget->deleteLater();
  // emit delete signal
  emit removeActivity(m_id);
}

void ActivityWidget::cancelRemove() {
  // hide the confirm widget
  m_layout->removeItem(m_removeWidget);
  // delete the confirm widget
  m_removeWidget->deleteLater();
}

void ActivityWidget::beginEdit() {
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
  m_layout->addItem(m_editWidget);
  // put the focus into the line edit
  m_lineEdit->setFocus(Qt::OtherFocusReason);
}

void ActivityWidget::acceptEdit() {
  // save the name
  QString name = m_lineEdit->text();
  // hide the edit widget
  m_layout->removeItem(m_editWidget);
  // delete the edit widget
  m_editWidget->deleteLater();
  // emit rename signal
  emit renameActivity(m_id, name);
}

void ActivityWidget::cancelEdit() {
  // hide the edit widget
  m_layout->removeItem(m_editWidget);
  // delete the edit widget
  m_editWidget->deleteLater();
}

void ActivityWidget::beginAdd() {
  // emit add signal
  emit addActivity(m_id);
}
