#include "dialogfournisseur.h"
#include "connexion.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connexion c;
    bool verifConnexion = c.ouvrirConnexion();
    Dialogfournisseur w;

    if(verifConnexion){
    w.show();
    QMessageBox::information (nullptr, QObject::tr("CONNEXION"),
                QObject::tr("\n DATABASE IS OPEN  !\n"), QMessageBox::Ok);
    }
    else {
        QMessageBox::critical (nullptr, QObject::tr("CONNEXION"),
                    QObject::tr("\n DATABASE IS NOT OPEN  !\n"), QMessageBox::Ok);
    }
    return a.exec();
}
