#ifndef DIALOGFOURNISSEUR_H
#define DIALOGFOURNISSEUR_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include "fournisseur.h"
namespace Ui {
class Dialogfournisseur;
}

class Dialogfournisseur : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogfournisseur(QWidget *parent = nullptr);
    ~Dialogfournisseur();
    void initialisation();

private slots:
    void on_ajoutForm_clicked();

    void on_f_ajouterBtn_clicked();

    void on_pushButton_clicked();

    void on_f_recherche_textChanged(const QString &arg1);

    void on_tableFournisseur_activated(const QModelIndex &index);

    void on_f_modifier_clicked();

    void on_f_supprimer_clicked();


     void sendMail();
     void mailSent(QString);
     void on_contactBtn_clicked();



     void on_pushButton_2_clicked();

     void on_btnPrint_clicked();

     void on_triBtn_clicked();

     void on_btn_Adresse_ajout_clicked();

     void on_ajouter_Adresse_clicked();

     void on_supprimer_Adresse_clicked();

private:
    Ui::Dialogfournisseur *ui;
    fournisseur f;
};

#endif // DIALOGFOURNISSEUR_H
