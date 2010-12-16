#include "activity.h"

#include <Plasma/IconWidget>

Activity::Activity(QString id, QGraphicsItem *parent) : QGraphicsWidget(parent), m_id(id) {
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
  layout->setOrientation(Qt::Horizontal);
  layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);
  // TODO: instead of a status icon use the activity icon and overlay the start/stop icons
  // create status icon
  m_status = new Plasma::IconWidget(this);
  m_status->setOrientation(Qt::Horizontal);
  m_status->setIcon("media-playback-start");
  m_status->setMinimumHeight(32);
  m_status->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addItem(m_status);
  // create label
  m_label = new Plasma::IconWidget(this);
  m_label->setOrientation(Qt::Horizontal);
  m_label->setMinimumHeight(32);
  m_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  layout->addItem(m_label);
  // toggle running state when clicked on the icon
  connect(m_status, SIGNAL(clicked()), this, SLOT(toggleStatus()));
  // setCurrent the activity when clicked on the name
  connect(m_label, SIGNAL(clicked()), this, SLOT(setCurrent()));
}

Activity::~Activity() {
}

QString Activity::name() {
  return m_name;
}

void Activity::setName(QString name) {
  m_name = name;
  // update label
  m_label->setText(m_name);
}

void Activity::setIcon(QString icon) {
  // update label
  if (icon != "")
    m_label->setIcon(icon);
  else
    m_label->setIcon("plasma");
}

void Activity::setCurrent(bool current) {
  // update label
  QFont font = m_label->font();
  if (current)
    font.setWeight(QFont::Bold);
  else
    font.setWeight(QFont::Normal);
  m_label->setFont(font);
}

void Activity::setState(QString state) {
  m_state = state;
  // update icon
  if (m_state == "Stopped")
    m_status->setIcon("media-playback-start");
  else
    m_status->setIcon("media-playback-stop");
}

void Activity::setCurrent() {
  emit setCurrent(m_id);
}

void Activity::toggleStatus() {
  if (m_state == "Stopped")
    emit startActivity(m_id);
  else
    emit stopActivity(m_id);
}
