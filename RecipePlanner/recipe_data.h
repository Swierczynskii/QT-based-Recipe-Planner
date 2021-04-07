#ifndef RECIPE_DATA_H
#define RECIPE_DATA_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
//#include <QDebug>
#include <QMessageBox>
#include "recipe.h"
//#include "mainwindow.h"

class recipe_data
{
public:
    recipe_data(QString filename);
    ~recipe_data();

    void initialize_data(QString filename);
    QList<recipe> read_recipes_data();
    recipe get_recipe(QString recipe_name);
    void save_to_recipe_list(QString name, QJsonArray description, QMap<QString, QString> ingredients);
    void write_to_file(QString filename);
public slots:
    void delete_recipe(QString recipe_name, QString filename);
private:
    QJsonObject data_recipe;
    QList<recipe> recipe_list;
};

#endif // RECIPE_DATA_H
