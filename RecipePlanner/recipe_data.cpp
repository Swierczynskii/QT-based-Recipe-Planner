#include "recipe_data.h"
#include <QMessageBox>
#include <QDebug>

recipe_data::recipe_data(QString filename){initialize_data(filename);}
recipe_data::~recipe_data(){}

void recipe_data::initialize_data(QString filename){

    //QString filename = "/home/jasiek01/EGUI/lab1/RecipePlanner/recipe.json";
    //QString filename = windowFilePath();
    QFile file;
    file.setFileName(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QString file_data = file.readAll();
    file.close();
    QJsonDocument json_doc = QJsonDocument::fromJson(file_data.toUtf8());
    data_recipe = json_doc.object();

    this->recipe_list = read_recipes_data();

}

recipe recipe_data::get_recipe(QString recipe_name)
{
    if(recipe_name.isEmpty())
        return recipe();

    foreach (const auto recipe, this->recipe_list)
    {
        if(recipe.get_name() == recipe_name)
        {
                return recipe;
        }
    }
}

QList<recipe> recipe_data::read_recipes_data(){

    if(this->data_recipe.isEmpty())
        return QList<recipe>();

    QList<recipe> recipe_list1;
    QMap<QString, QString> ingredients;
    QJsonArray recipe_description;

    foreach(const QString &it, this->data_recipe.keys() )
    {
        ingredients.clear();
        QJsonObject header = this->data_recipe.value(it).toObject();

        foreach (const QString &it2, header.keys())
        {

            if (it2 == "recipe")
            {
                recipe_description = header.value(it2).toArray();
            }
            else
            {
                QString ingr_specs = header.value(it2).toString();
                ingredients.insert(it2, ingr_specs);
            }
        }
            recipe *recipe1 = new recipe(it, recipe_description, ingredients);
            recipe_list1.append(*recipe1);
    }
    return recipe_list1;
}

void recipe_data::save_to_recipe_list(QString name, QJsonArray description, QMap<QString, QString> ingredients){

    recipe *new_recipe = new recipe(name, description, ingredients);
    this->recipe_list.append(*new_recipe);
}

void recipe_data::write_to_file(QString filename){

    //QString filename = "/home/jasiek01/EGUI/lab1/RecipePlanner/recipe.json";
    //QString filename = windowFilePath();
    QFile file;
    file.setFileName(filename);

    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        return;
    }

    QJsonObject obj;
    foreach (const auto recipe, this->recipe_list) {
        QJsonObject obj2;
        obj2["recipe"] = recipe.get_description();
        QMap<QString, QString>::iterator it;
        QMap<QString, QString> ingredients = recipe.get_ingredients();
        for (it = ingredients.begin();
             it != ingredients.end(); ++it)
        {
            obj2[it.key()] = it.value();
        }
        obj[recipe.get_name()] = obj2;
    }
    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

}

void recipe_data::delete_recipe(QString recipe_name, QString filename){
    if(recipe_name.isEmpty())
        return;

    int i = 0;
    foreach (const auto &recipe, this->recipe_list)
    {
        if(recipe.get_name() == recipe_name)
        {
                //recipe.delete_single_recipe();
                recipe_list.removeAt(i);
        }
        i++;
    }
    write_to_file(filename);
}
