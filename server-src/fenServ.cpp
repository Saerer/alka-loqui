#include "fenserv.h"

FenServ::FenServ()
{
    //Disposition des boutons de la fenÃªtre
    servStat = new QLabel;
    buSent = new QPushButton(tr("Envoyer"));
    textSer = new QTextEdit;
    ligneCom = new QLineEdit;
    quitBu = new QPushButton(tr("Quitter"));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout2 = new QHBoxLayout;
    layout->addWidget(servStat);
    layout->addWidget(textSer);
    layout->addLayout(layout2);
    layout2->addWidget(ligneCom);
    layout2->addWidget(buSent);
    layout2->addWidget(quitBu);
    setLayout(layout);

    setWindowTitle(tr("Alka-Loqui//SERVEUR"));
//Definition des connection
    connect(quitBu, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(buSent, SIGNAL(clicked()), this, SLOT());  //Plsu qu'a faire le slot pour envoyer la commande

    //Parametrage des widgets
    textSer->setReadOnly(true);

    setWindowTitle(tr("Alka-Loqui//SERVEUR"));

    //Gérance du serveur

    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, 42424)) //Tout adresse IP et sur port 42424
    {
        servStat->setText(tr("Problème de démarrage serveur dû à <br />") + server->errorString());
    }
    else
    {
        servStat->setText(tr("Démarrage accompli avec succès sur le port <strong>")+ QString::number(server->serverPort()) + tr("</strong>.<br />Les clients peuvent se connecter"));
        connect(server, SIGNAL(newConnection()), this, SLOT(newCon()));
    }

    tailleMessage = 0;
}

void FenServ::newCon()
    {
        sentAll(tr("<strong>Quelqu'un vient de se connecter.</strong>"));

        QTcpSocket *newGuest = server->nextPendingConnection();
        guests<< newGuest;
        connect(newGuest, SIGNAL(readyRead()), this, SLOT(dataRec()));
        connect(newGuest, SIGNAL(disconnected()), this, SLOT(discGuest()));
    }
void FenServ::dataRec()
    {
        QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
        if (socket == 0)
            return;

        QDataStream paqEnt(socket);

        if (tailleMessage == 0)
        {
            if (socket->bytesAvailable() < (int)sizeof(quint16))
                return;

            paqEnt >> tailleMessage;
        }

        if (socket->bytesAvailable() < tailleMessage)
            return;


        QString text;
        paqEnt >> text;
        sentAll(text);

        tailleMessage=0;
    }
void FenServ::discGuest()
{
    sentAll(tr("<strong>RIP quelqu'un nous a quitté</strong>"));

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == 0)
        return;

    guests.removeOne(socket);
    socket->deleteLater();
}
void FenServ::sentAll(const QString &text)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << text;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    for (int i = 0; i < guests.size(); i++)
    {
        guests[i]->write(paquet);
    }

}

