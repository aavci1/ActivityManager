#include "activitymanager.h"

#include "activitywidget.h"

#include <Plasma/Extender>
#include <Plasma/ExtenderItem>
#include <Plasma/Service>
#include <Plasma/ServiceJob>

#include <QGraphicsLinearLayout>
#include <QString>

ActivityManager::ActivityManager(QObject *parent, const QVariantList &args): Plasma::PopupApplet(parent, args) {
  setPopupIcon("preferences-activities");
  setAspectRatioMode(Plasma::IgnoreAspectRatio);
}

void ActivityManager::init() {
  extender()->setEmptyExtenderMessage(i18n("No Activities..."));
  // don't grow too much height
  extender()->setMaximumHeight(300);
  if (extender()->item("Activities") == 0) {
    // create the item
    Plasma::ExtenderItem *item = new Plasma::ExtenderItem(extender());
    initExtenderItem(item);
    // set item name and title
    item->setName("Activities");
    item->setTitle("Activities");
  }
  // connect data sources
  Plasma::DataEngine *engine = dataEngine("org.kde.activities");
  foreach (const QString source, engine->sources())
    activityAdded(source);
  // activity addition and removal
  connect(engine, SIGNAL(sourceAdded(QString)), this, SLOT(activityAdded(QString)));
  connect(engine, SIGNAL(sourceRemoved(QString)), this, SLOT(activityRemoved(QString)));
}

void ActivityManager::initExtenderItem(Plasma::ExtenderItem *item) {
  // create the widget
  QGraphicsWidget *widget = new QGraphicsWidget(this);
  // TODO: use the size of the longest activity name
  widget->setPreferredWidth(350);
  // create the layout
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(widget);
  layout->setOrientation(Qt::Vertical);
  widget->setLayout(layout);
  // set up the widget
  item->setWidget(widget);
}

void ActivityManager::dataUpdated(QString source, Plasma::DataEngine::Data data) {
  if (!m_activities.contains(source))
    return;
  ActivityWidget *activity = m_activities[source];
  // update activity info
  activity->setName(data["Name"].toString());
  activity->setState(data["State"].toString());
  activity->setIcon(data["Icon"].toString());
  activity->setCurrent(data["Current"].toBool());
  // sort activities
  sortActivities();
}

void ActivityManager::activityAdded(QString id) {
  // skip the Status source
  if (id == "Status")
    return;
  // create a new activity object
  ActivityWidget *activity = new ActivityWidget(extender()->item("Activities"), id);
  // add activity to the list
  m_activities.insert(id, activity);
  // connect activity update signal
  dataEngine("org.kde.activities")->connectSource(id, this);
  // connect activity start/stop signals
  connect(activity, SIGNAL(setCurrent(QString)), this, SLOT(setCurrent(QString)));
  connect(activity, SIGNAL(startActivity(QString)), this, SLOT(start(QString)));
  connect(activity, SIGNAL(stopActivity(QString)), this, SLOT(stop(QString)));
  connect(activity, SIGNAL(addActivity(QString)), this, SLOT(add(QString)));
  connect(activity, SIGNAL(removeActivity(QString)), this, SLOT(remove(QString)));
  connect(activity, SIGNAL(renameActivity(QString,QString)), this, SLOT(setName(QString,QString)));
}

void ActivityManager::activityRemoved(QString id) {
  if (!m_activities.contains(id))
    return;
  // delete the activity
  delete m_activities.take(id);
}

void ActivityManager::add(QString id) {
  add(id, "New Activity");
}

void ActivityManager::add(QString id, QString name) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  KConfigGroup op = service->operationDescription("add");
  op.writeEntry("Name", name);
  Plasma::ServiceJob *job = service->startOperationCall(op);
  connect(job, SIGNAL(finished(KJob*)), service, SLOT(deleteLater()));
}

void ActivityManager::setCurrent(QString id) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  Plasma::ServiceJob *job = service->startOperationCall(service->operationDescription("setCurrent"));
  connect(job, SIGNAL(finished(KJob*)), service, SLOT(deleteLater()));
}

void ActivityManager::stop(QString id) {
  // TODO: when activity is stopped, take a screenshot and use that icon
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  Plasma::ServiceJob *job = service->startOperationCall(service->operationDescription("stop"));
  connect(job, SIGNAL(finished(KJob*)), service, SLOT(deleteLater()));
}

void ActivityManager::start(QString id) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  Plasma::ServiceJob *job = service->startOperationCall(service->operationDescription("start"));
  connect(job, SIGNAL(finished(KJob*)), service, SLOT(deleteLater()));
}

void ActivityManager::setName(QString id, QString name) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  KConfigGroup op = service->operationDescription("setName");
  op.writeEntry("Name", name);
  Plasma::ServiceJob *job = service->startOperationCall(op);
  connect(job, SIGNAL(finished(KJob*)), service, SLOT(deleteLater()));
}

void ActivityManager::setIcon(QString id, QString name) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  KConfigGroup op = service->operationDescription("setIcon");
  op.writeEntry("Icon", name);
  Plasma::ServiceJob *job = service->startOperationCall(op);
  connect(job, SIGNAL(finished(KJob*)), service, SLOT(deleteLater()));
}

void ActivityManager::remove(QString id) {
  Plasma::Service *service = dataEngine("org.kde.activities")->serviceForSource(id);
  KConfigGroup op = service->operationDescription("remove");
  op.writeEntry("Id", id);
  Plasma::ServiceJob *job = service->startOperationCall(op);
  connect(job, SIGNAL(finished(KJob*)), service, SLOT(deleteLater()));
}

void ActivityManager::sortActivities() {
  QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout *>(static_cast<QGraphicsWidget *>(extender()->item("Activities")->widget())->layout());
  // remove all activities from the layout
  while (layout->count())
    layout->removeAt(0);
  // insert all activities to a map
  QMap<QString, ActivityWidget *> activities;
  foreach (ActivityWidget *activity, m_activities.values())
    activities.insertMulti(activity->name(), activity);
  // add activities into the layout at ascending order of their names
  foreach (ActivityWidget *activity, activities.values())
    layout->addItem(activity);
}

K_EXPORT_PLASMA_APPLET(activitymanager, ActivityManager)
