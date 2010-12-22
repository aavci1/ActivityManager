#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <Plasma/DataEngine>
#include <Plasma/PopupApplet>

#include <QHash>

class ActivityWidget;

class QAction;

namespace Plasma {
  class ExtenderItem;
};

class ActivityManager : public Plasma::PopupApplet {
  Q_OBJECT
public:
  ActivityManager(QObject *, const QVariantList &);

  void init();
  void initExtenderItem(Plasma::ExtenderItem *item);

public slots:
  void toolTipAboutToShow();
  void dataUpdated(QString source, Plasma::DataEngine::Data data);
  void activityAdded(QString id);
  void activityRemoved(QString id);

  void add(QString id);
  void add(QString id, QString name);
  void setCurrent(QString id);
  void stop(QString id);
  void start(QString id);
  void setName(QString id, QString name);
  void setIcon(QString id, QString name);
  void remove(QString id);

private slots:
  void toggleLock();

private:
  void sortActivities();

  QHash<QString, ActivityWidget *> m_activities;
  QAction *toggleLockAction;
  QString m_currentName;
  QString m_currentIcon;
};

#endif
