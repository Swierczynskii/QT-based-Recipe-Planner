#include "recipe.h"

recipe::recipe(){}
recipe::~recipe(){}

recipe::recipe(QString recipe_name, QJsonArray recipe_description
               , QMap<QString, QString> ingredients){

    this->recipe_name = recipe_name;
    this->recipe_description = recipe_description;
    this->ingredients = ingredients;
}

recipe::recipe(QString recipe_name, QJsonArray recipe_description){
    this->recipe_description = recipe_description;
    this->recipe_name = recipe_name;
}

