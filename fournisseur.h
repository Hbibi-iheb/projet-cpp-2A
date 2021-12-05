#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H
#include <QSqlQueryModel>
#include <QString>

class fournisseur
{
public:
    fournisseur();
    fournisseur(int,QString,QString,int,QString);
    int getCin(){return cin;}
    QString getNom(){return nom;}
     QString getPrenom(){return prenom;}
      QString getDateNaissance(){return datedenaissance;}
    int getTel(){return numtel;}
    bool ajouter();
    bool modifier(int);
    bool supprimer(int);
    QSqlQueryModel * afficher();
    QSqlQueryModel *afficherRech(QString res);
    QSqlQueryModel *afficherTri(int t);
    int verifierAdresse(int ff);



private:
    int cin,numtel;
    QString nom,prenom,datedenaissance;
};

#endif // FOURNISSEUR_H
