#include "shopping_list.h"
#include "ui_shopping_list.h"
#include <QMessageBox>

shopping_list::shopping_list(recipe_data *data_recipe, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shopping_list)
{
    ui->setupUi(this);
    setWindowTitle("Shopping List Calculator");
    this->data_recipe = data_recipe;
    load_recipes_names();
    model1 = new QStandardItemModel(this);
    model1->clear();
    ui->tableView->setModel(model1);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    model1->setHorizontalHeaderLabels(headers);
}

shopping_list::~shopping_list()
{
    delete ui;
}

void shopping_list::load_recipes_names(){

    model = new QStringListModel(this);
    QString filename = "/home/jasiek01/EGUI/lab1/RecipePlanner/recipe.json";
    QFile file;
    file.setFileName(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this, "ERROR", "Cannot open file with recipes: " + file.errorString());
    }
    else{
        QString jsonStr = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson((jsonStr.toUtf8()));
        QJsonObject obj = doc.object();
    if(obj.empty()){
        QStringList list;
        list << "List of recipes is empty.";
        model->setStringList(list);
        ui->comboBox->setModel(model);
    }
    else{
        QStringList list_of_recipes;
        for (auto it = obj.begin(); it != obj.end(); it++){
            QString recipe_header = it.key().toUtf8();
            list_of_recipes << recipe_header;
        }
        model->setStringList(list_of_recipes);
        ui->comboBox->setModel(model);
    }
    }
}

void shopping_list::on_pushButton_clicked()
{
    model = new QStringListModel(this);
    //add recipe name to the list
    QString box_recipe_name = ui->comboBox->currentText();
    box_names_list << box_recipe_name;
    model->setStringList(box_names_list);
    ui->listView->setModel(model);
}

void shopping_list::on_pushButton_2_clicked()
{
    if(model1->rowCount() != 0){
        QMessageBox::critical(this, "ERROR", "Calculation has been already done");
        return;
    }

    QString recipe1;
    QMap<QString, QString> editable_ingredients;
    QMap<QString, QString> to_add_ingredients;
    QMap<QString, QString> to_add_ingredients2;

    for (int j = 0; j < model->rowCount(); j++){
        recipe1 = ui->listView->model()->index(j, 0).data().toString();
        recipe editable_recipe = data_recipe->get_recipe(recipe1);
        editable_ingredients = editable_recipe.get_ingredients();
        QMap<QString, QString>::iterator it;
        for (it = editable_ingredients.begin();
             it != editable_ingredients.end(); ++it)
        {
            QString type = it.key();
            QMap<QString, QString>::iterator i;
            QString quant_unit = it.value();
            for (i = to_add_ingredients.begin();
                 i != to_add_ingredients.end(); ++i)
            {
                if (type == i.key()){
                    QStringList q_u_list = quant_unit.split(" ");
                    QStringList q_u_list2 = i.value().split(" ");
                    QString quant = q_u_list.value(0);
                    QString quant2 = q_u_list2.value(0);
                    QString unit = q_u_list.value(1);
                    QString unit2 = q_u_list2.value(1);
                    if( unit == unit2){
                        float q = quant.toFloat();
                        float q2 = quant2.toFloat();
                        float q3 = q+q2;
                        QString update = QString::number(q3);
                        quant_unit = update + " " + unit;
                    }
                    else{
                        type = type + " ";
                    }
                }
            }
            to_add_ingredients.insert(type, quant_unit);

        }
    }
    //to_add_ingredients = to_add_ingredients+to_add_ingredients2;
    QMap<QString, QString>::iterator it2;
    for (it2 = to_add_ingredients.begin();
         it2 != to_add_ingredients.end(); ++it2)
    {
        QList<QStandardItem *> ingredients;
        QString type = it2.key();
        QStringList quant_unit = it2.value().split(" ");
        QString quant = quant_unit.value(0);
        QString unit = quant_unit.value(1);
        ingredients.append(new QStandardItem(type));
        ingredients.append(new QStandardItem(quant));
        ingredients.append(new QStandardItem(unit));
        model1->appendRow(ingredients);
    }

}

void shopping_list::on_pushButton_3_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    int row = index.row();

    box_names_list.removeAt(row);
    this->model->removeRow(row);
}
