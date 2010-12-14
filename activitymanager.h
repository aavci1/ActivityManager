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

  QGraphicsWidget *graphicsWidget() { return m_widget; }

public slots:
  void dataUpdated(QString source, Plasma::DataEngine::Data data);
  void activityAdded(QString id);
  void activityRemoved(QString id);

  void setCurrent(QString id);
  void stopActivity(QString id);
  void startActivity(QString id);

private:
  QGraphicsWidget *m_widget;
  QMap<QString, Activity *> m_activities;
};

#endif
