#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QGraphicsLinearLayout>
#include <QString>

namespace Plasma {
  class IconWidget;
};

class Activity : public QObject {
  Q_OBJECT
public:
  Activity(QString id);
  ~Activity();

  QGraphicsLinearLayout *layout();

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
  QGraphicsLinearLayout *m_layout;
  Plasma::IconWidget *m_status;
  Plasma::IconWidget *m_label;
  QString m_id;
  QString m_state;
};

#endif
