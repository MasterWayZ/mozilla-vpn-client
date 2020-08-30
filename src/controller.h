#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "connectionhealth.h"
#include "controllerimpl.h"
#include "server.h"

#include <QObject>
#include <QTimer>

class MozillaVPN;

class Controller final : public QObject
{
    Q_OBJECT

public:
    enum State {
        StateOff,
        StateConnecting,
        StateOn,
        StateDisconnecting,
        StateSwitching,
    };
    Q_ENUM(State)

private:
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(int time READ time NOTIFY timeChanged)
    Q_PROPERTY(QString currentCity READ currentCity NOTIFY stateChanged)
    Q_PROPERTY(QString switchingCity READ switchingCity NOTIFY stateChanged)

public:
    Controller();

    void setVPN(MozillaVPN *vpn);

    State state() const { return m_state; }

    Q_INVOKABLE void activate();

    Q_INVOKABLE void deactivate();

    Q_INVOKABLE void changeServer(const QString &countryCode, const QString &city);

    Q_INVOKABLE void quit();

    Q_INVOKABLE void logout();

    int time() const { return m_time; }

    const QString &currentCity() const { return m_currentCity; }

    const QString &switchingCity() const { return m_switchingCity; }

private Q_SLOTS:
    void connected();
    void disconnected();
    void timeUpdated();

signals:
    void stateChanged();
    void timeChanged();
    void readyToQuit();

private:
    State m_state = StateOff;

    MozillaVPN *m_vpn = nullptr;

    QTimer m_timer;
    int m_time = 0;

    QScopedPointer<ControllerImpl> m_impl;

    QString m_currentCity;
    Server m_currentServer;

    QString m_switchingCountryCode;
    QString m_switchingCity;

    ConnectionHealth m_connectionHealth;

    enum NextStep {
        None,
        Quit,
        Disconnect,
    };

    NextStep m_nextStep = None;
};

#endif // CONTROLLER_H
