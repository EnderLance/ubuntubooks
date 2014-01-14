/*
 * Copyright (C) 2012-2013 Joseph Mills.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// FIX ME add to the HEader
#include "oauth2.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
OAuth2::OAuth2(QObject *parent)
    : QObject(parent)
{
    m_pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}



QString OAuth2::clientId()const
{
    return m_strClientID;
}
void OAuth2::setClientId(const QString &clientId)
{
    m_strClientID = clientId;
}

QString OAuth2::clientSecert()const
{
    return m_strClientSecret;
}
void OAuth2::setClientSecert( const QString &clientSecert)
{
    m_strClientSecret = clientSecert;
}

QString OAuth2::redirectURI()const
{
    return m_strRedirectURI;
}
void OAuth2::setRedirectURI(const QString &redirectURI)
{
    m_strRedirectURI = redirectURI;
}

QString OAuth2::scopes()const
{
    return m_strScope;
}
void OAuth2::setScopes(const QString &scope)
{
    m_strScope = scope;
}

QString OAuth2::endPoint()const
{
    return m_strEndPoint;
}
void OAuth2::setEndPoint(const QString &endPoint)
{
    m_strEndPoint = endPoint;
}


void OAuth2::accessTokenObtained()
{
    emit loginDone();
}

void OAuth2::codeObtained()
{
    QUrl url("https://accounts.google.com/o/oauth2/token");
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QString str = "client_id=" + m_strClientID;
    str += "&redirect_uri=" + m_strRedirectURI;
    str += "&client_secret=" + m_strClientSecret;
    str += "&grant_type=authorization_code";
    str += "&code=" + m_strCode;

    QByteArray params = str.toLatin1();
    m_pNetworkAccessManager->post(request, params);
}

void OAuth2::replyFinished(QNetworkReply* reply)
{
    QString json = reply->readAll();
//    qDebug() << "Reply = " << json;
    QJsonDocument jsonDocument  = QJsonDocument::fromJson(json.toUtf8());
    if( jsonDocument.isEmpty() )
    {
        emit errorOccured(QString("Some thing went wrong with creating the json document"));
        return;
    }
    QJsonObject jsonObject = jsonDocument.object();

    QString atId;
    atId = "access_token";
    m_strAccessToken =  jsonObject.value(QString(atId) ).toString();

    QString refToken;
    refToken = "refresh_token";
    m_strRefreshToken = jsonObject.value(QString(refToken)).toString();

    QString tokenType;
    tokenType = "token_type";
    m_strTokenType = jsonObject.value(QString(tokenType)).toString();

    QString exIn;
    exIn= "expires_in";
    m_strExpiresIn = jsonObject.value(QString(exIn)).toDouble();

    QString idToken;
    idToken = "id_token";
    m_strIdToken = jsonObject.value(QString(idToken)).toString();

    emit loginDone();
}


QString OAuth2::getloginUrl()
{
    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=code&scope=%4&approval_prompt=force&access_type=offline").arg(m_strEndPoint).arg(m_strClientID).
            arg(m_strRedirectURI).arg(m_strScope);
            return str;
}

QString OAuth2::getaccessToken()
{
    return m_strAccessToken;
}

QString OAuth2::getTokenType()
{
    return m_strTokenType;
}

double OAuth2::getExperationTime()
{
    return m_strExpiresIn;
}

QString OAuth2::getTokenId()
{
    return m_strIdToken;
}

QString OAuth2::getRefreshToken()
{
    return m_strRefreshToken;
}

bool OAuth2::isAuthorized()
{
    return m_strAccessToken != "";
}

void OAuth2::startLogin()
{
//    qDebug() << "OAuth2::startLogin";
    //FIXME add a switch here that sees if all is filled out correctly
    //        qDebug() << "Warning" << "To work with application you need to register your own application in <b>Google</b>.\n" << "Learn more from <a href=\"http://code.google.com/p/qt-google-plus/wiki/HowToRegisterYourApplicationInGoogle\">here</a>";
    //        return;
    //    }
        getAccessTokenFromRefreshToken();
}

void OAuth2::getAccessTokenFromRefreshToken()
{
    QUrl url("https://accounts.google.com/o/oauth2/token");
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QString str = "client_id=" + m_strClientID;
    str += "&client_secret=" + m_strClientSecret;
    str += "&grant_type=refresh_token";
    str += "&refresh_token=" + m_strRefreshToken;

    QByteArray params = str.toLatin1();
    m_pNetworkAccessManager->post(request, params);
}

void OAuth2::urlChanged(const QUrl &url)
{
    QString str = url.toString();
    if(str.indexOf("access_token") != -1)
    {
        QStringList query = str.split("#");
        QStringList lst = query[1].split("&");
        for (int i=0; i<lst.count(); i++ )
        {
            QStringList pair = lst[i].split("=");
            if (pair[0] == "access_token")
            {
                m_strAccessToken = pair[1];
                emit accessTokenObtained();
            }
        }
    }
    else if(str.indexOf("code=") != -1)
    {
        QStringList query = str.split("?");
        QStringList lst = query[1].split("&");
        for (int i=0; i<lst.count(); i++ )
        {
            QStringList pair = lst[i].split("=");
            if (pair[0] == "code")
            {
                m_strCode = pair[1];
                emit codeObtained();
            }
        }
    }
}
