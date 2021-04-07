#ifndef RECIPE_DIALOG_H
#define RECIPE_DIALOG_H

#include <QDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
//#include <QtCore>
#include <QtGui>
#include "recipe_data.h"

namespace Ui {
class recipe_dialog;
}

class recipe_dialog : public QDialog
{
    Q_OBJECT
    const QStringList headers = (QStringList() << "TYPE"
                                                   << "QUANTITY"
                                                   << "UNIT");

public:
    explicit recipe_dialog(recipe_data *data_recipe, QString recipe_name, QString filename, QWidget *parent = nullptr);
    ~recipe_dialog();

private slots:
    void on_add_clicked();

    void on_save_clicked();

    void on_delete_2_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void initialize_dialog(QString recipe_name);

    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::recipe_dialog *ui;
    QStandardItemModel *model;
    recipe_data *data_recipe;

    QJsonArray editable_description;
    QMap<QString, QString> editable_ingredients;
    QString recip_name;
    QString name;
    QString file_path;
    QJsonArray description;
};

#endif // RECIPE_DIALOG_H
