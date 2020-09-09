#ifndef WEATHERHANDLE_H
#define WEATHERHANDLE_H

#include <QObject>

class WeatherHandle : public QObject
{
    Q_OBJECT
public:

    explicit WeatherHandle(QObject *parent = nullptr);
    ~WeatherHandle();
void checkNetState();
void getIPGeoAdress();
signals:
void analysisIPGeoAddr(QString totalGeoAddr);
public slots:
private:
QString getIPGeoAdress(QString str);
QString totalGeoAddr;

};

#endif // WEATHERHANDLE_H
