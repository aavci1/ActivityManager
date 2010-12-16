#include "activitywidget.h"

#include <Plasma/IconWidget>

#include <QGraphicsLinearLayout>

ActivityWidget::ActivityWidget(QString id, QGraphicsItem *parent) : QGraphicsWidget(parent), m_id(id) {
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
  layout->setOrientation(Qt::Horizontal);
  layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);
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
  connect(m_removeIcon, SIGNAL(clicked()), this, SLOT(removeSelf()));
}

ActivityWidget::~ActivityWidget() {
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

void ActivityWidget::removeSelf() {
  // TODO: ask for confirmation
  emit removeActivity(m_id);
}
