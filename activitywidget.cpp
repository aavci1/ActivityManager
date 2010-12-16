#include "activitywidget.h"

#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/PushButton>

#include <QGraphicsLinearLayout>

ActivityWidget::ActivityWidget(QString id, QGraphicsItem *parent) : QGraphicsWidget(parent), m_layout(0), m_removeWidget(0), m_label(0), m_stateIcon(0), m_removeIcon(0), m_id(id) {
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
  m_stateIcon->setPreferredSize(24, 24);
  m_stateIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addItem(m_stateIcon);
  layout->setAlignment(m_stateIcon, Qt::AlignCenter);
  // toggle running state when clicked on the icon
  connect(m_stateIcon, SIGNAL(clicked()), this, SLOT(toggleStatus()));
  // create status icon
  m_removeIcon = new Plasma::IconWidget(this);
  m_removeIcon->setOrientation(Qt::Horizontal);
  m_removeIcon->setIcon("edit-delete");
  m_removeIcon->setPreferredSize(16, 16);
  m_removeIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addItem(m_removeIcon);
  layout->setAlignment(m_removeIcon, Qt::AlignCenter);
  // delete activity when clicked on the delete icon
  connect(m_removeIcon, SIGNAL(clicked()), this, SLOT(confirmRemove()));
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

void ActivityWidget::confirmRemove() {
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
