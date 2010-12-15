#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <Plasma/DataEngine>
#include <Plasma/ExtenderItem>
#include <Plasma/PopupApplet>

#include <QGraphicsWidget>
#include <QMap>

class Activity;

class ActivityManager : public Plasma::PopupApplet {
  Q_OBJECT
public:
  ActivityManager(QObject *, const QVariantList &);

  void init();
  void initExtenderItem(Plasma::ExtenderItem *item);

public slots:
  void dataUpdated(QString source, Plasma::DataEngine::Data data);
  void activityAdded(QString id);
  void activityRemoved(QString id);

  void add(QString id, QString name);
  void setCurrent(QString id);
  void stop(QString id);
  void start(QString id);
  void setName(QString id, QString name);
  void setIcon(QString id, QString name);
  void remove(QString id);

private:
  QGraphicsWidget *m_widget;
  QMap<QString, Activity *> m_activities;
};

#endif
