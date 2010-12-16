#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QGraphicsWidget>
#include <QString>

namespace Plasma {
  class IconWidget;
};

class Activity : public QGraphicsWidget {
  Q_OBJECT
public:
  Activity(QString id, QGraphicsItem *parent = 0);
  ~Activity();

  QString name();
  void setName(QString name);
  void setIcon(QString icon);
  void setCurrent(bool current);
  void setState(QString state);

public slots:
  void setCurrent();
  void toggleStatus();

signals:
  void setCurrent(QString id);
  void startActivity(QString id);
  void stopActivity(QString id);

private:
  Plasma::IconWidget *m_stateIcon;
  Plasma::IconWidget *m_label;
  QString m_id;
  QString m_name;
  QString m_state;
};

#endif
