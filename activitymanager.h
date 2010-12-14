#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <Plasma/DataEngine>
#include <Plasma/PopupApplet>

#include <QMap>
#include <QGraphicsWidget>

class Activity;

class ActivityManager : public Plasma::PopupApplet {
  Q_OBJECT
public:
  ActivityManager(QObject *, const QVariantList &);

  void init();

  void addActivity(QString id);
  void removeActivity(QString id);

  QGraphicsWidget *graphicsWidget() { return m_widget; }

public slots:
  void dataUpdated(QString source, Plasma::DataEngine::Data data);
  void sourceAdded(QString source);
  void sourceRemoved(QString source);
  void startActivity(QString id);
  void stopActivity(QString id);
  void setCurrent(QString id);

private:
  QGraphicsWidget *m_widget;
  QMap<QString, Activity *> m_activities;
};

#endif
