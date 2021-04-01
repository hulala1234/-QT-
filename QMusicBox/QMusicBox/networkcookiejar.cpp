// networkcookiejar.cpp
#include "networkcookiejar.h"
#include<qDebug>
NetworkCookieJar::NetworkCookieJar()
{
}

QList<QNetworkCookie> NetworkCookieJar::getCookies()
{

    return allCookies();
}

void NetworkCookieJar::setCookies(const QList<QNetworkCookie>& cookieList)
{
    if(this == NULL) {
        return;
    }
    setAllCookies(cookieList);
}
