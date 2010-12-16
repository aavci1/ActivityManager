#ifndef ACTIVITYWIDGET_H
#define ACTIVITYWIDGET_H

#include <QGraphicsWidget>
#include <QString>

namespace Plasma {
  class IconWidget;
};

class ActivityWidget : public QGraphicsWidget {
  Q_OBJECT
public:
  ActivityWidget(QString id, QGraphicsItem *parent = 0);
  ~ActivityWidget();

  QString name();
  void setName(QString name);
  void setIcon(QString icon);
  void setCurrent(bool current);
  void setState(QString state);

public slots:
  void setCurrent();
  void toggleStatus();
  void removeSelf();

signals:
  void setCurrent(QString id);
  void startActivity(QString id);
  void stopActivity(QString id);
  void removeActivity(QString id);

private:
  Plasma::IconWidget *m_label;
  Plasma::IconWidget *m_stateIcon;
  Plasma::IconWidget *m_removeIcon;
  QString m_id;
  QString m_name;
  QString m_state;
};

#endif
