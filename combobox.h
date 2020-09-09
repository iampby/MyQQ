#ifndef COMBOBOX_H
#define COMBOBOX_H
//复合框 对于城市
#include <QObject>
#include<qcombobox.h>

class ComboBox : public QComboBox
{
    Q_OBJECT
public:
    ComboBox(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent*e);
    void hidePopup();
    void showPopup();
    void setText(const QString&);
    QString text()const;
 public   slots:

signals:
    void clicked();
    void emitHidePopup();
private:
   bool isLeftButtonPressed;
QString m_text;
};

#endif // COMBOBOX_H
