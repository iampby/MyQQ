#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H
/*
����ͼƬ�� qml textedit ������ ��Դ csdn
��ַ�� https://blog.csdn.net/u011283226/article/details/88559370
*/

#include <QMovie>
#include <QTextCursor>
#include <QQuickWindow>
#include"images.h"
class Api : public QObject
{
    Q_OBJECT

public:
    Api(QObject *parent = nullptr);

    Q_INVOKABLE bool exists(const QString &arg);
    Q_INVOKABLE QString baseName(const QString &arg);
};

class QQuickTextDocument;
class ImageHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument* document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)
    //���ڿ��Ʋ����ͼƬ������/��
    Q_PROPERTY(int maxWidth READ maxWidth WRITE setMaxWidth NOTIFY maxWidthChanged)
    Q_PROPERTY(int maxHeight READ maxHeight WRITE setMaxHeight NOTIFY maxHeightChanged)

public:
    ImageHelper(QObject *parent = nullptr);
    ~ImageHelper();

    Q_INVOKABLE void insertImage(const QUrl &url);
    //�޸�Դ���
    Q_INVOKABLE void insertImage(QPixmap pix, QString number, QString myqq);//����ͼƬ��ָ��·�����ļ��� count����
    Q_INVOKABLE QString htmlAdjsut(QString html) const;//�������͵�html����

    Q_INVOKABLE void cleanup();

    QQuickTextDocument* document() const;
    void setDocument(QQuickTextDocument *document);

    int cursorPosition() const;
    void setCursorPosition(int position);

    int selectionStart() const;
    void setSelectionStart(int position);

    int selectionEnd() const;
    void setSelectionEnd(int position);

    int maxWidth() const;
    void setMaxWidth(int max);

    int maxHeight() const;
    void setMaxHeight(int max);

signals:
    void needUpdate();
    void documentChanged();
    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();
    void maxWidthChanged();
    void maxHeightChanged();

private:
    QTextDocument *textDocument() const;
    QTextCursor textCursor() const;

private:
    QHash<QUrl, QMovie *> m_urls;
    QQuickTextDocument *m_document;
    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;
    int m_maxWidth;
    int m_maxHeight;
    //�޸�Դ���
    quint64 count=0;//���ظ�����
};

#endif // IMAGEHELPER_H
