#pragma execution_character_set("utf-8")
#include "imagehelper.h"
#include <QFile>
#include <QFileInfo>
#include <QQmlFile>
#include <QQuickTextDocument>
#include <QDebug>
#include <QDir>
#include <qapplication.h>
#include <QClipboard>
#include<qmimedata.h>
#include<qbuffer.h>
#include <qregularexpression.h>

Api::Api(QObject *parent)
    : QObject(parent)
{
}

bool Api::exists(const QString &arg)
{
    return QFile::exists(arg);
}

QString Api::baseName(const QString &arg)
{
    return QFileInfo(arg).baseName();
}

ImageHelper::ImageHelper(QObject *parent)
    : QObject(parent),
      m_maxWidth(150),
      m_maxHeight(150)
{

}

ImageHelper::~ImageHelper()
{
    cleanup();
}

void ImageHelper::insertImage(const QUrl &url)
{
    QImage image = QImage(QQmlFile::urlToLocalFileOrQrc(url));
    if (image.isNull())
    {
        qDebug() << "不支持的图像格式";
        return;
    }
    qint32 width=image.width(),height=image.height();
    QString filename = url.toString();
    QString suffix = QFileInfo(filename).suffix();
    if (suffix == "GIF" || suffix == "gif") //如果是gif，则单独处理
    {
        QString gif = filename;
        if (gif.left(4) == "file")
            gif = gif.mid(8);
        else if (gif.left(3) == "qrc")
            gif = gif.mid(3);

        textCursor().insertHtml("<img src='" + url.toString() + "' width = " +
                                QString::number(qMin(m_maxWidth, width)) + " height = " +
                                QString::number(qMin(m_maxHeight, height))+ "/>");
        textDocument()->addResource(QTextDocument::ImageResource, url, image);
        if (m_urls.contains(url))
            return;
        else
        {
            QMovie *movie = new QMovie(gif);
            movie->setCacheMode(QMovie::CacheNone);
            connect(movie, &QMovie::finished, movie, &QMovie::start);   //循环播放
            connect(movie, &QMovie::frameChanged, this, [url, this](int)
            {
                QMovie *movie = qobject_cast<QMovie *>(sender());
                textDocument()->addResource(QTextDocument::ImageResource, url, movie->currentPixmap());
                emit needUpdate();
            });
            m_urls[url] = movie;
            movie->start();
        }
    }
    else
    {
        if(width>m_maxWidth||height>m_maxHeight){
            image=image.scaled(m_maxWidth,m_maxHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        }
        QTextImageFormat format;
        format.setName(filename);
        format.setWidth( image.width());
        format.setHeight(image.height());
        textCursor().insertImage(format, QTextFrameFormat::InFlow);
    }
}

void ImageHelper::insertImage(QPixmap pix,QString number,QString myqq)
{
    if(pix.isNull())return;
    QDir dir("../user/"+myqq+"/friends/"+number+"/chat/temp/");
    if(!dir.exists())dir.mkpath("./");
    if(!pix.save(dir.filePath(QString("%1").arg(++count)),"png")){
        qDebug()<<"saved a pixmap is of failure";
        return;
    }
    QImage image=pix.toImage();
    QTextImageFormat format;
    format.setName("file:///"+dir.absoluteFilePath(QString("%1").arg(count)));
    if(image.width()>m_maxWidth||image.height()>m_maxHeight){
        image=image.scaled(m_maxWidth,m_maxHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    format.setWidth(image.width());
    format.setHeight(image.height());
    textCursor().insertImage(format, QTextFrameFormat::InFlow);
    //把数据保存到
    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *data=new QMimeData;

    QByteArray tdata;
    //图片插入剪贴板
    QString html="<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" />"
                 "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head>"
                 "<body>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><!--StartFragment-->"
                 "<img src=\"file:///"+dir.absoluteFilePath(QString("%1").arg(count))+"\" width=\""+QString::number(image.width())+
            "\" height=\""+QString::number(image.height())+"\" /><!--EndFragment--></p></body></html>";
    tdata=html.toUtf8();
    data->setData("text/html",tdata);
    clipboard->setMimeData(data);
}

QString ImageHelper::htmlAdjsut(QString html)const
{
    //qDebug()<<html<<endl<<endl;
    QString r=QString(),tr=QString();
    QRegExp reg("(?:<p.*</p>)");

    qint32 pos=reg.indexIn(html);
    if(pos==-1){
        qDebug()<<"document content is not correct to html";
        return html;
    }
    r=reg.cap();//保存p内容
    reg.setPattern(">");
    pos=reg.indexIn(r);
    if(pos==-1){
        qDebug()<<"document content is not correct to html";
        return html;
    }
    r=r.right(r.length()-pos-1);//去掉格式内容
    reg.setPattern("(?:<img src=\\\".* />)");//非贪婪匹配
    pos=r.indexOf(reg);
    if(pos==-1){
        qDebug()<<"this document is a plain text";
        return html;
    }
    QRegularExpression ex("(?:<img src=\\\"file:///)(.*)(?:\\\" width=\\\")(.*)(?:\\\" height=\\\")(.*)(?:\\\" />)");
    ex.setPatternOptions(QRegularExpression::InvertedGreedinessOption);//非贪婪匹配
    QRegularExpressionMatchIterator iteror=  ex.globalMatch(r);
    tr=r;
    while(iteror.hasNext()){
        QRegularExpressionMatch item=  iteror.next();
        QString name=item.captured(1);
        QPixmap pix(name);
        int width=pix.width();
        int height=pix.height();
        if(width==0||height==0){
            qDebug()<<"warning:a pixmap is empty in rich text";
            continue;
        }
        if(width>height){
            if(width>290){
                height=height*1.0/width*290;
                width=290;
            }
        }else{
            if(height>290){
                width=width*1.0/height*290;
                height=290;
            }
        }
        QString o=item.captured(),n=o;

        n.replace(QRegExp("width=\\\""+item.captured(2)+"\\\""),QString("width=\"%1\"").arg(width));
        n.replace(QRegExp("height=\\\""+item.captured(2)+"\\\""),QString("height=\"%1\"").arg(height));
        n="<br /&f>"+n+"<br /&f>";
        r.replace(o,n);
    }
    r.replace("<br /&f><br />","<br />");
    r.replace("<br /><br /&f>","<br />");
    r.replace("<br /&f><br /&f>","<br />");
    r.replace("<br /&f>","<br />");
    html.replace(tr,r);
    //qDebug()<<html;
    return html;
}



void ImageHelper::cleanup()
{
    for (auto it : m_urls)
        delete it,it=nullptr;
    m_urls.clear();
}



QQuickTextDocument* ImageHelper::document() const
{
    return  m_document;
}

void ImageHelper::setDocument(QQuickTextDocument *document)
{
    if (document != m_document)
    {
        m_document = document;
        emit documentChanged();
    }
}

int ImageHelper::cursorPosition() const
{
    return m_cursorPosition;
}

void ImageHelper::setCursorPosition(int position)
{
    if (position != m_cursorPosition)
    {
        m_cursorPosition = position;
        emit cursorPositionChanged();
    }
}

int ImageHelper::selectionStart() const
{
    return m_selectionStart;
}

void ImageHelper::setSelectionStart(int position)
{
    if (position != m_selectionStart)
    {
        m_selectionStart = position;
        emit selectionStartChanged();
    }
}

int ImageHelper::selectionEnd() const
{
    return m_selectionEnd;
}

void ImageHelper::setSelectionEnd(int position)
{
    if (position != m_selectionEnd)
    {
        m_selectionEnd = position;
        emit selectionEndChanged();
    }
}

int ImageHelper::maxWidth() const
{
    return m_maxWidth;
}

void ImageHelper::setMaxWidth(int max)
{
    if (max != m_maxWidth)
    {
        m_maxWidth = max;
        emit maxWidthChanged();
    }
}

int ImageHelper::maxHeight() const
{
    return m_maxHeight;
}

void ImageHelper::setMaxHeight(int max)
{
    if (max != m_maxHeight)
    {
        m_maxHeight = max;
        emit maxHeightChanged();
    }
}

QTextDocument* ImageHelper::textDocument() const
{
    if (m_document)
        return m_document->textDocument();
    else return nullptr;
}

QTextCursor ImageHelper::textCursor() const
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(doc);
    if (m_selectionStart != m_selectionEnd)
    {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    }
    else
    {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}
