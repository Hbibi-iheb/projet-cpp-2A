#include "fournisseur.h"
#include <QSqlQuery>
fournisseur::fournisseur()
{
    int cin =0;
    QString nom ="";
    QString prenom="";
    QString datenaissance="";
    int numtel = 0;
}

fournisseur::fournisseur(int cin,QString nom,QString prenom,int numtel,QString ddn){
    this->cin=cin;
    this->nom=nom;
    this->prenom=prenom;
    this->numtel = numtel;
    this->datedenaissance = ddn;

}
bool fournisseur::ajouter(){
    QSqlQuery query;
    query.prepare("INSERT INTO fournisseur (cin,nom,prenom,numtel,datedenaissance)"
                "Values(:cin,:nom,:prenom,:numtel,:datedenaissance)");
    query.bindValue(":cin",cin);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":numtel",numtel);
    query.bindValue(":datedenaissance",datedenaissance);
   return  query.exec();
}

bool fournisseur::supprimer(int cin)
{
    QString res=QString::number(cin);
    QSqlQuery query;
    query.prepare("Delete from fournisseur where cin=:cin ");
    query.bindValue(":cin",res);
    return query.exec();
}
bool fournisseur::modifier(int cin){
    QString res=QString::number(cin);
    QSqlQuery query;
    query.prepare("update fournisseur set nom=:nom,prenom=:prenom,numtel=:numtel,datedenaissance=:datedenaissance "
                    "  where cin=:cin");
    query.bindValue(":cin",res);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":numtel",numtel);
    query.bindValue(":datedenaissance",datedenaissance);
   return  query.exec();
}


QSqlQueryModel * fournisseur::afficher()
{QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM fournisseur order by cin ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TELLEPHONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("NAISSANCE"));
    return model;
}


QSqlQueryModel * fournisseur::afficherRech(QString res)
{
    QSqlQueryModel *model=new QSqlQueryModel();

    model->setQuery("select * from fournisseur where cin like'"+res+"%' or nom like'"+res+"%' or prenom like'"+res+"%' or numtel like'"+res+"%'");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TELLEPHONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("NAISSANCE"));
    return model;
}
QSqlQueryModel * fournisseur::afficherTri(int t)
{
    QSqlQueryModel *model=new QSqlQueryModel();

    switch (t){

        case 0:
        model->setQuery("select * from fournisseur order by cin ");
        break;
    case 1 :

        model->setQuery("select * from fournisseur order by nom ");
break;
  case 2 :
        model->setQuery("select * from fournisseur order by prenom ");
break;
    case 3:
        model->setQuery("select * from fournisseur order by Numtel ");
break;
    }



    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TELLEPHONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("NAISSANCE"));
    return model;
}
int fournisseur::verifierAdresse(int tt){
    QSqlQuery q;

    int test = 0;
    q.prepare("select count(*) from ADRESSE_FOURNISSEUR  where  FOURNISSEUR_ID=:four");
    q.bindValue(":four",tt);

    if(q.exec()){
        while(q.next()){
            test = q.value(0).toInt();
        }
    }
    return test;



}
