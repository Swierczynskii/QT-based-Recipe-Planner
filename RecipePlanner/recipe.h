#ifndef RECIPE_H
#define RECIPE_H
#include <QString>
#include <QMap>
#include <QJsonArray>

class recipe
{
public:
    recipe();
    ~recipe();
    recipe(QString recipe_name, QJsonArray recipe_description
                   , QMap<QString, QString> ingredients);
    recipe(QString recipe_name, QJsonArray recipe_description);

    QString get_name() const {return recipe_name;}
    QJsonArray get_description() const {return recipe_description;}
    QMap<QString, QString> get_ingredients() const {return ingredients;}
    //void delete_single_recipe();
private:
    QString recipe_name;
    QJsonArray recipe_description;
    QMap<QString, QString> ingredients;
};

#endif // RECIPE_H
