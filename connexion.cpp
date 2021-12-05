#include "connexion.h"

connexion::connexion()
{

}
bool connexion::ouvrirConnexion()
{
    bool testOuverture=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("ihebqt");
    db.setUserName("ihebqt");
    db.setPassword("ihebqt");
    if (db.open())
        testOuverture=true;
    return  testOuverture;
}
void connexion::fermerConnexion()
{db.close();}
