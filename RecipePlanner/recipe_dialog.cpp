#include "recipe_dialog.h"
#include "ui_recipe_dialog.h"
#include <QMessageBox>

recipe_dialog::recipe_dialog(recipe_data *data_recipe, QString recipe_name, QString filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recipe_dialog)
{
    ui->setupUi(this);
    this->data_recipe = data_recipe;
    setWindowTitle("Recipe");
    model = new QStandardItemModel(this);
    model->clear();
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->listView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->listView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    model->setHorizontalHeaderLabels(headers);
    file_path = filename;
    recip_name = recipe_name;
    initialize_dialog(recipe_name);
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(on_tableView_activated(QModelIndex)));

}


void recipe_dialog::initialize_dialog(QString recipe_name){

    if(recipe_name == NULL){
        ui->name->setText("Enter dish's name here.");
        ui->description_edit->setText("\nDescribe your recipe here!"
                                      "\n\n\n\n\nAdd ingredients below ↓");
    }
    else
    {
        recipe editable_recipe = data_recipe->get_recipe(recipe_name);
        ui->name->setText(editable_recipe.get_name());
        this->editable_description = editable_recipe.get_description();
        this->editable_ingredients = editable_recipe.get_ingredients();

        QString editable_text;
        int i = 0;
        for (QJsonArray::iterator it = editable_description.begin();
             it != editable_description.end(); it++)
        {
            editable_text.append(editable_description.at(i).toString());
            editable_text.append("\n");
            i++;
        }

        ui->description_edit->setText(editable_text);
        QMap<QString, QString>::iterator it;
        for (it = editable_ingredients.begin();
             it != editable_ingredients.end(); ++it)
        {
            QList<QStandardItem *> ingredients;
            ingredients.append(new QStandardItem(it.key()));
            QStringList quant_unit = it.value().split(" ");
            QString quant = quant_unit.value(0);
            QString unit = quant_unit.value(1);
            ingredients.append(new QStandardItem(quant));
            ingredients.append(new QStandardItem(unit));

            model->appendRow(ingredients);
        }
    }

}


recipe_dialog::~recipe_dialog()
{
    delete ui;
}

void recipe_dialog::on_add_clicked()
{
    QList<QStandardItem *> ingredient;
    QString ingr_type = ui->type->text();
    QString ingr_quant = ui->quantity->text();
    QString ingr_unit = ui->unit->text();

    if(ingr_type == nullptr || ingr_quant == nullptr || ingr_unit == nullptr){
        QMessageBox::critical(this, "ERROR", "Cannot add ingredient with any slot being empty.");
    }
    else if(model->findItems(ingr_type).isEmpty()){
        ingredient.append(new QStandardItem(ingr_type));
        ingredient.append(new QStandardItem(ingr_quant));
        ingredient.append(new QStandardItem(ingr_unit));

        model->appendRow(ingredient);
    }
    else{
        QMessageBox::critical(this, "ERROR", "Cannot add the same ingredient again.");
    }
}

void recipe_dialog::on_save_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    int row = index.row();
    QList<QStandardItem *> ingredient;
    QString ingr_type = ui->type->text();
    QString ingr_quant = ui->quantity->text();
    QString ingr_unit = ui->unit->text();
    model->removeRow(row);
    if(ingr_type == nullptr || ingr_quant == nullptr || ingr_unit == nullptr){
        QMessageBox::critical(this, "ERROR", "Cannot add ingredient with any slot being empty.");
    }
    else if(model->findItems(ingr_type).isEmpty()){
        ingredient.append(new QStandardItem(ingr_type));
        ingredient.append(new QStandardItem(ingr_quant));
        ingredient.append(new QStandardItem(ingr_unit));

        model->insertRow(row, ingredient);
    }
    else{
        QMessageBox::critical(this, "ERROR", "Cannot add the same ingredient again.");
    }
}

void recipe_dialog::on_delete_2_clicked()   // try to mark whole row while clicking
{
    model->removeRows(ui->tableView->currentIndex().row(),1);
}

void recipe_dialog::on_buttonBox_accepted()
{
    // reading recipes name
    this->name = ui->name->text();
    // writing description into QJsonArray
    QString recipe_decription = ui->description_edit->toPlainText();
    QStringList description_list = recipe_decription.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    this->description = QJsonArray::fromStringList(description_list);
    //
    if(name == nullptr || recipe_decription == nullptr || model->rowCount() == 0){
          QMessageBox::critical(this, "ERROR", "Cannot add recipe with any slot being empty.\n");
          return;
          //on_buttonBox_rejected();
    }
    //Creatin saved_ingredients map
    QMap<QString, QString> saved_ingredients;
    QStringList quant_type;
    QString junction, junction1, junction2;
    for (int j = 0; j < model->rowCount(); j++){

       junction1 = ui->tableView->model()->index(j, 1).data().toString();
       junction2 = ui->tableView->model()->index(j, 2).data().toString();
       junction = junction1 + " " + junction2;

       quant_type.append(junction);
    }

    for (int i = 0; i < model->rowCount(); i++){
        saved_ingredients.insert(ui->tableView->model()->index(i, 0).data().toString(), quant_type.at(i));
    }

    if (name != recip_name){
        this->data_recipe->delete_recipe(recip_name, windowFilePath());
    }
    this->data_recipe->save_to_recipe_list(name, description, saved_ingredients);
    this->data_recipe->write_to_file(file_path);
    this->data_recipe->initialize_data(file_path);
    emit accepted();
}

void recipe_dialog::on_buttonBox_rejected()
{
    emit rejected();
}


void recipe_dialog::on_tableView_activated(const QModelIndex &index)
{
    //QString val = ui->tableView->model()->data(index).toString();

    QString type_ = ui->tableView->model()->index(index.row(), 0).data().toString();
    QString quantity_ = ui->tableView->model()->index(index.row(), 1).data().toString();
    QString unit_ = ui->tableView->model()->index(index.row(), 2).data().toString();

    ui->type->setText(type_);
    ui->quantity->setText(quantity_);
    ui->unit->setText(unit_);

    //on_save_clicked();
}
