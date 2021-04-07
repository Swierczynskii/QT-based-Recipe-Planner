#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_actionJSON_triggered();
    data_recipe = new recipe_data(windowFilePath());
    //data_recipe->initialize_data();
    load_recipes_names();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_recipes_names(){

    model = new QStringListModel(this);
    //QString filename = "/home/jasiek01/EGUI/lab1/RecipePlanner/recipe.json";
    QString filename = windowFilePath();
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
            ui->listView->setModel(model);
        }
        else{
            QStringList list_of_recipes;
            for (auto it = obj.begin(); it != obj.end(); it++){
                QString recipe_header = it.key().toUtf8();
                list_of_recipes << recipe_header;
            }
            model->setStringList(list_of_recipes);
            ui->listView->setModel(model);
        }
    }
}

void MainWindow::on_add_recipe_clicked() // bug-> if the name exists on the list already the program overwrites it.
{
    addRecipe = new recipe_dialog(this->data_recipe, NULL, windowFilePath(), this);
    this->model->removeRows(0, model->rowCount());
    connect(addRecipe, SIGNAL(accepted()), this, SLOT(load_recipes_names()));
    connect(addRecipe, SIGNAL(rejected()), this, SLOT(load_recipes_names()));
    addRecipe->show();
}

void MainWindow::on_edit_recipe_clicked()
{
    //edit recipe
    QModelIndex index = ui->listView->currentIndex();
    QString selected_name = index.data(Qt::DisplayRole).toString();
    if(selected_name == NULL){
        QMessageBox::critical(this, "ERROR",
                              "None recipe was selected");
        return;
    }
    addRecipe = new recipe_dialog(this->data_recipe, selected_name, windowFilePath(), this);
    this->model->removeRows(0, model->rowCount());
    //this->data_recipe->delete_recipe(selected_name);
    //connect(addRecipe, SIGNAL(accepted()), this, SLOT(delete_recipe()));
    connect(addRecipe, SIGNAL(accepted()), this, SLOT(load_recipes_names()));
    connect(addRecipe, SIGNAL(rejected()), this, SLOT(load_recipes_names()));
    addRecipe->show();
}



void MainWindow::on_delete_recipe_clicked()
{   //delete recipe
    QModelIndex index = ui->listView->currentIndex();
    QString selected_name = index.data(Qt::DisplayRole).toString();
    if(selected_name == NULL){
        QMessageBox::critical(this, "ERROR",
                              "None recipe was selected");
        return;
    }
    this->data_recipe->delete_recipe(selected_name, windowFilePath());
    this->model->removeRows(0, model->rowCount());
    load_recipes_names();
}


void MainWindow::on_actionShop_List_triggered()
{
    new_shopping_list = new shopping_list(this->data_recipe, this);
    new_shopping_list->show();
}

void MainWindow::on_actionJSON_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"));


    QFile file(filename);
    setWindowFilePath(filename);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file with recipes: " + file.errorString());
    }
    //setWindowFilePath(filename);
    file.close();
    load_recipes_names();
}
