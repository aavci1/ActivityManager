#include "activitymanager.h"

#include "activity.h"

#include <Plasma/Extender>
#include <Plasma/Service>
#include <Plasma/ServiceJob>

#include <QGraphicsLinearLayout>
#include <QString>

ActivityManager::ActivityManager(QObject *parent, const QVariantList &args): Plasma::PopupApplet(parent, args), m_widget(0) {
  setPopupIcon("plasma");
  setAspectRatioMode(Plasma::IgnoreAspectRatio);
}

void ActivityManager::init() {
  extender()->setEmptyExtenderMessage(i18n("No Activities..."));
  // add an extender item
  Plasma::ExtenderItem *item = new Plasma::ExtenderItem(extender());
  initExtenderItem(item);
  // give item a title
  item->setTitle("Activities");
  // connect data sources
  Plasma::DataEngine *engine = dataEngine("org.kde.activities");
  foreach (const QString source, engine->sources())
    if (source != "Status")
      activityAdded(source);
  // activity addition and removal
  connect(engine, SIGNAL(sourceAdded(QString)), this, SLOT(activityAdded(QString)));
  connect(engine, SIGNAL(sourceRemoved(QString)), this, SLOT(activityRemoved(QString)));
}

void ActivityManager::initExtenderItem(Plasma::ExtenderItem *item) {
  if (m_widget == 0) {
    // create the widget
    m_widget = new QGraphicsWidget(this);
    m_widget->setMinimumSize(QSizeF(250, 45));
    m_widget->setPreferredSize(QSizeF(250, 45));
    // create the layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(m_widget);
    layout->setOrientation(Qt::Vertical);
    m_widget->setLayout(layout);
  }
  // set up the widget
  item->setWidget(m_widget);
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
  // HACK: correctly update minimum height without using hardcoded numbers
  // update widget minimum size
  extender()->setMinimumHeight(m_activities.size() * 38 + 40);
}

void ActivityManager::activityRemoved(QString id) {
  if (!m_activities.contains(id))
    return;
  // delete the activity
  delete m_activities.take(id);
  // HACK: correctly update minimum height without using hardcoded numbers
  // update widget minimum size
  extender()->setMinimumHeight(m_activities.size() * 38 + 40);
  extender()->setPreferredHeight(m_activities.size() * 38 + 40);
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
  Plasma::ServiceJob *job = service->startOperationCall(service->operationDescription("remove"));
  connect(job, SIGNAL(finished(KJob*)), service, SLOT(deleteLater()));
}

K_EXPORT_PLASMA_APPLET(activitymanager, ActivityManager)
