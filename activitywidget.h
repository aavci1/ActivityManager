#ifndef ACTIVITYWIDGET_H
#define ACTIVITYWIDGET_H

#include <QGraphicsWidget>
#include <QString>

namespace Plasma {
  class IconWidget;
  class LineEdit;
};

class QGraphicsLinearLayout;

class ActivityWidget : public QGraphicsWidget {
  Q_OBJECT
public:
  ActivityWidget(QString id, QGraphicsItem *parent = 0);

  QString name();
  void setName(QString name);
  void setIcon(QString icon);
  void setCurrent(bool current);
  void setState(QString state);

public slots:
  void setCurrent();
  void toggleStatus();
  void beginRemove();
  void acceptRemove();
  void cancelRemove();
  void beginEdit();
  void acceptEdit();
  void cancelEdit();

signals:
  void setCurrent(QString id);
  void startActivity(QString id);
  void stopActivity(QString id);
  void removeActivity(QString id);
  void renameActivity(QString id, QString name);

private:
  QGraphicsLinearLayout *m_layout;
  QGraphicsWidget *m_removeWidget;
  QGraphicsWidget *m_editWidget;
  Plasma::LineEdit *m_lineEdit;
  Plasma::IconWidget *m_label;
  Plasma::IconWidget *m_stateIcon;
  Plasma::IconWidget *m_editIcon;
  Plasma::IconWidget *m_removeIcon;
  QString m_id;
  QString m_name;
  QString m_state;
};

#endif
