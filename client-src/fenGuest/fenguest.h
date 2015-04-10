#ifndef FENGUEST_H
#define FENGUEST_H

#include <QtWidgets>
#include <QtNetwork>

extern int guest_working;

class FenGuest : public QWidget
{
    Q_OBJECT
public:
    FenGuest();

private slots:
    void static enterPressedCo();
    void static dataRec();
    void static guestCo();
    void static guestDisc();
    void static erreurSocket(QAbstractSocket::SocketError erreur);
    void static sentMessage(QString textYHTS);
    void static on_boutCo_clicked();

    void static on_boutSent_clicked();

private:

    static QTcpSocket *socket;
    static quint16 tailleMessage;
    //Element de la fenetre;
    static QPushButton *boutCo;
    static QLineEdit *adrIP;
    static QSpinBox *numPort;
    static QTextEdit *listeMessages;
    static QLineEdit *pseudo;
    static QLineEdit *textSe;
    static QPushButton *boutSent;

};

#endif // FENGUEST_H
