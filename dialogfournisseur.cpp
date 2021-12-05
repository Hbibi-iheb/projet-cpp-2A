#include "dialogfournisseur.h"
#include "ui_dialogfournisseur.h"
#include "smtp.h"
#include <QPainter>
#include <QPdfWriter>
#include <QDesktopServices>
#include <QUrl>

Dialogfournisseur::Dialogfournisseur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogfournisseur)
{
    ui->setupUi(this);
   connect(ui->enoyerMail, SIGNAL(clicked()),this, SLOT(sendMail()));

    initialisation(); // methode initialisation hide buttons + clear lineedits + hide lineedits
}

Dialogfournisseur::~Dialogfournisseur()
{
    delete ui;
}

void Dialogfournisseur::initialisation(){
    //Controle de saisie Adresse fournisseurstatus_adresse_label
    ui->btn_Adresse_ajout->hide();
    ui->supprimer_Adresse->hide();
    ui->stackedWidget_3->hide();
    ui->comboBox_VIlle->hide();
    ui->lineEdit_Rue->hide();
    ui->lineEdit_Zip->hide();
    ui->ajouter_Adresse->hide();
    ui->stackedWidget_3->setCurrentIndex(0);

    // remplissage du table Fournisseur
     ui->tableFournisseur->setModel(f.afficher());


     ui->f_cin->clear();
     ui->f_nom->clear();
     ui->f_prenom->clear();
     ui->f_tel->clear();
     ui->stackedWidget->setCurrentIndex(0);
     ui->f_cin_e_1->setDisabled(true);
     ui->f_nom_e_1->clear();
     ui->f_prenom_e->clear();
     ui->f_tel_e->clear();
     ui->f_cin_e_1->hide();
     ui->f_nom_e_1->hide();
     ui->f_prenom_e->hide();
     ui->f_tel_e->hide();
     ui->f_daten_e->hide();
     ui->f_modifier->hide();
     ui->f_supprimer->hide();
     ui->f_cin->setValidator(new QIntValidator(0,9999999,this)); // ID AJOUT CONTROLE SAISIE
     ui->f_nom->setValidator(new QRegExpValidator( QRegExp("[A-Za-z ]*") , this )); // NOM AJOUT CONTROLE SAISIE
     ui->f_nom_e_1->setValidator(new QRegExpValidator( QRegExp("[A-Za-z ]*") , this )); // NOM MODIFIER CONTROLE SIAISE
     ui->f_prenom_e->setValidator(new QRegExpValidator( QRegExp("[A-Za-z ]*") , this )); // NOM MODIFIER CONTROLE SIAISE
     ui->f_prenom->setValidator(new QRegExpValidator( QRegExp("[A-Za-z ]*") , this )); // NOM MODIFIER CONTROLE SIAISE
     ui->f_tel->setValidator(new QIntValidator(0,99999999,this)); // PRIX MODIFICATION CONTROLE SAISIE
     ui->f_tel_e->setValidator(new QIntValidator(0,99999999,this)); // PRIX AJOUT CONTROLE SAISIE

}

void Dialogfournisseur::on_ajoutForm_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Dialogfournisseur::on_f_ajouterBtn_clicked()
{
    int cin = ui->f_cin->text().toInt();
    QString nom = ui->f_nom->text();
    QString prenom = ui->f_prenom->text();
    int tel = ui->f_tel->text().toInt();
    QString daten = ui->f_date->date().toString("dd/MM/yyyy");
    QString telf = QString::number(tel);
    if (cin<=0){
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur ajout"),
                                      QObject::tr("Verifier le champ Cin!! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else if(nom.size()<=2){
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur ajout"),
                                      QObject::tr("Le nom doit etre plus que 2 characteres!! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else if(prenom.size()<=2){
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur ajout"),
                                      QObject::tr("Le prenom doit etre plus que 2 characteres!! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else if(telf.size()<8 or telf.size()>8  ){
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur ajout"),
                                      QObject::tr("Le num tel doit etre 8 chiffres .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else{
    fournisseur four(cin,nom,prenom,tel,daten);
    bool validation = four.ajouter();
    if (validation){
        initialisation();
        QMessageBox::information(nullptr, QObject::tr("Fournisseur ajout"),
                                      QObject::tr("Fournisseur ajouté !! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else{
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur ajout"),
                                      QObject::tr("CIN/TEL deja utiliser!! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    }
}

void Dialogfournisseur::on_pushButton_clicked()
{

}

void Dialogfournisseur::on_f_recherche_textChanged(const QString &arg1)
{
    ui->tableFournisseur->setModel(f.afficherRech(arg1));
}

void Dialogfournisseur::on_tableFournisseur_activated(const QModelIndex &index)
{
    QString val=ui->tableFournisseur->model()->data(index).toString();
    int cin = val.toInt();
    int test = f.verifierAdresse(cin);
   if(test==0) {
       ui->stackedWidget_3->show();
       ui->status_adresse_label->show();
       ui->btn_Adresse_ajout->show();
        ui->status_adresse_label->setText("Le Fournisseur : "+val+" n'a pas d'adresse  ");
        ui->supprimer_Adresse->hide();
    }
    else if(test>0){
        QSqlQuery q;
        q.prepare("SELECT ville,rue,zip from adresse_fournisseur where fournisseur_id =:a ");
        q.bindValue(":a",cin);
        if(q.exec()){
            while(q.next()){
                ui->stackedWidget_3->show();
                ui->status_adresse_label->setText("L'adresse du fournisseur : "+val+" est : Ville  "+q.value(0).toString()+"Rue : " + q.value(1).toString()+" Zip : "+q.value(2).toString() );
                ui->supprimer_Adresse->show();
                ui->stackedWidget_3->show();
                ui->status_adresse_label->show();
                ui->btn_Adresse_ajout->hide();

            }
        }
    }
        QSqlQuery qry;
        qry.prepare("Select * from fournisseur where  cin='"+val+"'");
        if(qry.exec())
        {
            while(qry.next())
                    {
                        ui->f_cin_e_1->setText(val);
                        ui->f_nom_e_1->setText(qry.value(1).toString());
                        ui->f_prenom_e->setText(qry.value(2).toString());
                        ui->f_tel_e->setText(qry.value(3).toString());

                        ui->f_cin_e_1->setEnabled(false);
                        ui->f_nom_e_1->show();
                        ui->f_cin_e_1->show();
                        ui->f_prenom_e->show();
                        ui->f_tel_e->show();
                        ui->f_daten_e->show();
                        ui->f_modifier->show();
                        ui->f_supprimer->show();
                    }

        }
}

void Dialogfournisseur::on_f_modifier_clicked()
{
    int cin = ui->f_cin_e_1->text().toInt();
    QString nom = ui->f_nom_e_1->text();
    QString prenom = ui->f_prenom_e->text();
    int tel = ui->f_tel_e->text().toInt();
    QString daten = ui->f_daten_e->date().toString("dd/MM/yyyy");
    QString telf = QString::number(tel);
    if (cin<=0){
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur Modification"),
                                      QObject::tr("Verifier le champ Cin!! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else if(nom.size()<=2){
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur Modification"),
                                      QObject::tr("Le nom doit etre plus que 2 characteres!! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else if(prenom.size()<=2){
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur Modification"),
                                      QObject::tr("Le prenom doit etre plus que 2 characteres!! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else if(telf.size()<8 or telf.size()>8  ){
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur Modification"),
                                      QObject::tr("Le num tel doit etre 8 chiffres .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else{
    fournisseur four(cin,nom,prenom,tel,daten);
    bool validation = four.modifier(cin);
    if (validation){
        initialisation();
        QMessageBox::information(nullptr, QObject::tr("Fournisseur Modification"),
                                      QObject::tr("Fournisseur modifié !! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    else{
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur Modification"),
                                      QObject::tr("TEL deja utiliser!! .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);

    }
    }
}

void Dialogfournisseur::on_f_supprimer_clicked()
{
    int id=ui->f_cin_e_1->text().toInt();

    bool test2=f.supprimer(id);
    if(test2){
        initialisation();
        QMessageBox::information(nullptr, QObject::tr("Fournisseur Supprimer"),
                                      QObject::tr("Succés .\n" "Click Ok to exit."), QMessageBox::Ok);

    }
    else{
        initialisation();
        QMessageBox::critical(nullptr, QObject::tr("Fournisseur Supprimer"),
                                      QObject::tr("ERROR !\n"), QMessageBox::Cancel);

    }
}


void Dialogfournisseur::sendMail(){
  QString mail = ui->mailTo->text();
   QString objet =ui->mailObjet->text();
   QString body =ui->Mailbody->toPlainText();
   Smtp* smtp = new Smtp("iheb.hbibi@esprit.tn","hbibiesprit4396", "smtp.gmail.com",465);
   connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
   smtp->sendMail("iheb.hbibi@esprit.tn",mail,objet,body);
}



void Dialogfournisseur::mailSent(QString status)
    {
      if(status == "Message sent")
            QMessageBox::warning( nullptr, tr( "Qt Simple SMTP client" ), tr( "Message sent!\n\n" ) );
    }


void Dialogfournisseur::on_contactBtn_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(1);
}


void Dialogfournisseur::on_pushButton_2_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(0);
}

void Dialogfournisseur::on_btnPrint_clicked()
{
    QString nom=ui->f_nom_e_1->text()+".pdf";
    QString cin = ui->f_cin_e_1->text();
        QPdfWriter pdf("C:/Qt/"+cin+nom);

                       QPainter painter(&pdf);
                        // int i = 4000;
                              painter.setPen(Qt::blue);
                              painter.setFont(QFont("Arial", 12));
                              painter.drawText(2300,1200,"Fournisseur Numero "+cin+" confirmation");
                              painter.setPen(Qt::black);
                              painter.setFont(QFont("Arial", 12));
                             // painter.drawText(1100,2000,afficheDC);
                              painter.drawRect(1500,200,7300,2600);
                             // painter.drawRect(0,3000,9600,500);
                              painter.setFont(QFont("Arial",14));
                              painter.drawText(300,3300,"");
                                                      painter.drawText(2300,3300,"");
                                                      painter.drawText(4300,3300,"");
                                                      painter.drawText(6300,3300,"");
                                  painter.drawText(300,5000,"J'ai récus la demande de stock des produits");
                                 painter.drawText(300,6500,"Vos produits sont en cours de livraison");

                              int reponse = QMessageBox::question(this, "Génerer PDF", "<PDF Enregistré>...Vous Voulez Affichez Le PDF ?", QMessageBox::Yes |  QMessageBox::No);
                                  if (reponse == QMessageBox::Yes)
                                  {
                                      QDesktopServices::openUrl(QUrl::fromLocalFile("C:/Qt/"+cin+nom));

                                      painter.end();
                                  }
                                  if (reponse == QMessageBox::No)
                                  {
                                       painter.end();
                                  }
}

void Dialogfournisseur::on_triBtn_clicked()
{

    int type = ui->comboBoxTri->currentIndex();
    ui->tableFournisseur->setModel(f.afficherTri(type));
}

void Dialogfournisseur::on_btn_Adresse_ajout_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
    ui->comboBox_VIlle->show();
    ui->lineEdit_Rue->show();
    ui->lineEdit_Zip->show();
    ui->ajouter_Adresse->show();
}

void Dialogfournisseur::on_ajouter_Adresse_clicked()
{
    int id = 0;
    QString ville = ui->comboBox_VIlle->currentText();
    QString rue = ui->lineEdit_Rue->text();
    QString zip = ui->lineEdit_Zip->text();
    int FOURNISSEUR_ID = ui->f_cin_e_1->text().toInt();

    if (ville =="Ville"){
        QMessageBox::critical(nullptr, QObject::tr("Adresse Ajout"),
                                      QObject::tr("Ville invalide !\n"), QMessageBox::Cancel);
    }
    else{
    QSqlQuery q;
    q.prepare("select id from ADRESSE_FOURNISSEUR order by id asc");
    if(q.exec()){
        while(q.next()){
            if(q.value(0).toInt()==0){
                id = 1;
            }
            else if(q.value(0).toInt()>0){
                id = q.value(0).toInt()+1;
            }
        }
    }
    QSqlQuery q2;
    q2.prepare("insert into ADRESSE_FOURNISSEUR (id,FOURNISSEUR_ID,rue,zip,ville)"
               " values (:id,:FOURNISSEUR_ID,:rue,:zip,:ville)");
    q2.bindValue(":id",id);
    q2.bindValue(":FOURNISSEUR_ID",FOURNISSEUR_ID);
    q2.bindValue(":rue",rue);
    q2.bindValue(":zip",zip);
    q2.bindValue(":ville",ville);
    bool test = q2.exec();
    if(test){
         ui->stackedWidget_3->setCurrentIndex(0);
         initialisation();
        QMessageBox::information(nullptr, QObject::tr("Adresse Ajout"),
                                      QObject::tr("succés !\n"), QMessageBox::Cancel);
    }
    else{
        QMessageBox::critical(nullptr, QObject::tr("Adresse Ajout"),
                                      QObject::tr("Erreur !\n"), QMessageBox::Cancel);
    }
    }

}

void Dialogfournisseur::on_supprimer_Adresse_clicked()
{
    int fournisseur_id = ui->f_cin_e_1->text().toInt();
    QSqlQuery q;
    q.prepare("Delete from ADRESSE_FOURNISSEUR where FOURNISSEUR_ID=:id  ");
    q.bindValue(":id",fournisseur_id);
    bool test = q.exec();
    if(test){

         initialisation();
        QMessageBox::information(nullptr, QObject::tr("Adresse Suppression"),
                                      QObject::tr("succés !\n"), QMessageBox::Cancel);
    }
    else{
        initialisation();
        QMessageBox::critical(nullptr, QObject::tr("Adresse Suppression"),
                                      QObject::tr("Erreur !\n"), QMessageBox::Cancel);
    }
}
