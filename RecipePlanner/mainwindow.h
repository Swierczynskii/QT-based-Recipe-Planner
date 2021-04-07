#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "recipe_dialog.h"
#include "recipe_data.h"
#include "shopping_list.h"
#include <QMessageBox>
#include <QtGui>
#include <QFile>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void load_recipes_names();

    //void delete_recipe(QString selected_name);

private slots:
    void on_add_recipe_clicked();

    void on_edit_recipe_clicked();

    void on_delete_recipe_clicked();


    //void on_shoppinglist_clicked();

    void on_actionShop_List_triggered();

    void on_actionJSON_triggered();

private:
    Ui::MainWindow *ui;
    recipe_dialog *addRecipe;
    recipe_data *data_recipe;
    QStringListModel *model;
    QList<recipe> recipe_list;
    shopping_list *new_shopping_list;

};
#endif // MAINWINDOW_H
