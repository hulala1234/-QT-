// networkcookiejar.h
#ifndef NETWORKCOOKIEJAR_H
#define NETWORKCOOKIEJAR_H

#include <QNetworkCookie>
#include <QNetworkCookieJar>

class NetworkCookieJar : public QNetworkCookieJar
{
public:
    NetworkCookieJar();

    QList<QNetworkCookie> getCookies();
    void setCookies(const QList<QNetworkCookie>& cookieList);
};

#endif // NETWORKCOOKIEJAR_H
