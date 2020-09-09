#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H
/*
插入图片到 qml textedit 辅助类 来源 csdn
网址： https://blog.csdn.net/u011283226/article/details/88559370
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
    //用于控制插入的图片的最大宽/高
    Q_PROPERTY(int maxWidth READ maxWidth WRITE setMaxWidth NOTIFY maxWidthChanged)
    Q_PROPERTY(int maxHeight READ maxHeight WRITE setMaxHeight NOTIFY maxHeightChanged)

public:
    ImageHelper(QObject *parent = nullptr);
    ~ImageHelper();

    Q_INVOKABLE void insertImage(const QUrl &url);
    //修改源码段
    Q_INVOKABLE void insertImage(QPixmap pix, QString number, QString myqq);//插入图片到指定路径，文件名 count计数
    Q_INVOKABLE QString htmlAdjsut(QString html) const;//调整发送的html内容

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
    //修改源码段
    quint64 count=0;//不重复命名
};

#endif // IMAGEHELPER_H
