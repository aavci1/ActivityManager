#include "activitymanager.h"

#include "activity.h"

#include <Plasma/Service>

#include <QGraphicsLinearLayout>
#include <QString>

ActivityManager::ActivityManager(QObject *parent, const QVariantList &args): Plasma::PopupApplet(parent, args), m_widget(0) {
}

void ActivityManager::init() {
  setPopupIcon("plasma");
  // create popup
  m_widget = new QGraphicsWidget();
  // create a layout
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
  layout->setOrientation(Qt::Vertical);
  m_widget->setLayout(layout);
  // connect data sources
  Plasma::DataEngine *engine = dataEngine("org.kde.activities");
  foreach (const QString source, engine->sources())
    if (source != "Status")
      activityAdded(source);
  // activity addition and removal
  connect(engine, SIGNAL(sourceAdded(QString)), this, SLOT(activityAdded(QString)));
  connect(engine, SIGNAL(sourceRemoved(QString)), this, SLOT(activityRemoved(QString)));
}

void ActivityManager::dataUpdated(QString source, Plasma::DataEngine::Data data) {
  if (!m_activities.contains(source))
    return;
  // update activity info
  m_activities[source]->setName(data["Name"].toString());
  m_activities[source]->setState(data["State"].toString());
  m_activities[source]->setIcon(data["Icon"].toString());
  m_activities[source]->setCurrent(data["Current"].toBool());
}

void ActivityManager::activityAdded(QString id) {
  // create a new activity object
  Activity *activity = new Activity(id);
  // add activity to the list
  m_activities.insert(id, activity);
  // connect activity update signal
  dataEngine("org.kde.activities")->connectSource(id, this);
  // connect activity start/stop signals
  connect(activity, SIGNAL(startActivity(QString)), this, SLOT(start(QString)));
  connect(activity, SIGNAL(stopActivity(QString)), this, SLOT(stop(QString)));
  connect(activity, SIGNAL(setCurrent(QString)), this, SLOT(setCurrent(QString)));
  // show activity in the popup widget
  QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout *>(m_widget->layout());
  layout->addItem(activity->layout());
  // update widget minimum size
  m_widget->setMinimumSize(200, m_activities.size() * 38);
}

void ActivityManager::activityRemoved(QString id) {
  if (!m_activities.contains(id))
    return;
  // delete the activity
  delete m_activities.take(id);
  // update widget minimum size
  m_widget->setMinimumSize(200, m_activities.size() * 38);
}

void ActivityManager::add(QString id, QString name) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  KConfigGroup op = service->operationDescription("add");
  op.writeEntry("Name", name);
  service->startOperationCall(op);
}

void ActivityManager::setCurrent(QString id) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  service->startOperationCall(service->operationDescription("setCurrent"));
}

void ActivityManager::stop(QString id) {
  // TODO: when activity is stopped, take a screenshot and use that icon
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  service->startOperationCall(service->operationDescription("stop"));
}

void ActivityManager::start(QString id) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  service->startOperationCall(service->operationDescription("start"));
}

void ActivityManager::setName(QString id, QString name) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  KConfigGroup op = service->operationDescription("setName");
  op.writeEntry("Name", name);
  service->startOperationCall(op);
}

void ActivityManager::setIcon(QString id, QString name) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  KConfigGroup op = service->operationDescription("setIcon");
  op.writeEntry("Name", name);
  service->startOperationCall(op);
}

void ActivityManager::remove(QString id) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  KConfigGroup op = service->operationDescription("remove");
  op.writeEntry("Id", id);
  service->startOperationCall(op);
}

K_EXPORT_PLASMA_APPLET(activitymanager, ActivityManager)
