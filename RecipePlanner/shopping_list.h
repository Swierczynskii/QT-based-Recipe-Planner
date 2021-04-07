#ifndef SHOPPING_LIST_H
#define SHOPPING_LIST_H

#include <QtGui>
#include <QDialog>
#include "recipe_data.h"

namespace Ui {
class shopping_list;
}

class shopping_list : public QDialog
{
    Q_OBJECT
    const QStringList headers = (QStringList() << "TYPE"
                                                   << "QUANTITY"
                                                   << "UNIT");

public:
    explicit shopping_list(recipe_data *data_recipe, QWidget *parent = nullptr);
    ~shopping_list();
    //void load_recipes_names();

private slots:
    void on_pushButton_clicked();
    void load_recipes_names();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::shopping_list *ui;
    QStringListModel *model;
    QStandardItemModel *model1;
    //QStringListModel *model2;
    QStringList box_names_list;
    recipe_data *data_recipe;
};

#endif // SHOPPING_LIST_H
