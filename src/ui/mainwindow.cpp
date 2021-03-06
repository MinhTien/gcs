#include <QFile>        // for file operation
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SerialConfigurationWindow.h"
#include "ui_SerialConfigurationWindow.h"
#include "qextserialport.h"
//#include "qextserialenumerator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentStyle(GCS_MAINWINDOW_STYLE_INDOOR),
    styleFileName(QCoreApplication::applicationDirPath() + "style-indoor.css")

{
    ui->setupUi(this);
    ui->mainToolBar->addAction(ui->actionEngineer);
    ui->mainToolBar->addAction(ui->actionOperator);
    /** @brief Create ComSerialConfigurationWindow dialog*/
    m_SerialConfigWindow = new SerialSettingsWindow(this);
    //! [1]
//    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
//    m_port = new QextSerialPort(m_SerialConfigWindow->port_ui->portBox->currentText(), settings, QextSerialPort::EventDriven);
    //! [1]

    loadStyle(currentStyle);

    connect(ui->actionCom_Settings,SIGNAL(triggered()),SLOT(onComSettingTriggered()));
    connect(ui->actionConnect,SIGNAL(triggered()),SLOT(onComOpenCloseTriggered()));
    connect(ui->comOpenCloseButton,SIGNAL(clicked()),SLOT(comPortOpenCloseButtonClick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/** @brief For style control */
/** @brief Load a specific style */
void MainWindow::loadStyle(MainWindow::GCS_MAINWINDOW_STYLE style)
{
    switch (style) {
    case GCS_MAINWINDOW_STYLE_NATIVE: {
        // Native mode means setting no style
        // so if we were already in native mode
        // take no action
        // Only if a style was set, remove it.
        if (style != currentStyle) {
            qApp->setStyleSheet("");
            showInfoMessage(tr("Please restart QGroundControl"), tr("Please restart QGroundControl to switch to fully native look and feel. Currently you have loaded Qt's plastique style."));
        }
    }
    break;
    case GCS_MAINWINDOW_STYLE_INDOOR:
    qApp->setStyle("fusion");
        styleFileName = ":files/styles/style-indoor.css";
        reloadStylesheet();
        break;
    case GCS_MAINWINDOW_STYLE_OUTDOOR:
    qApp->setStyle("fusion");
        styleFileName = ":files/styles/style-outdoor.css";
        reloadStylesheet();
        break;
    }
    currentStyle = style;
}

/** @brief Reload the CSS style sheet */
void MainWindow::reloadStylesheet()
{
    // Load style sheet
    QFile* styleSheet = new QFile(styleFileName);
    if (!styleSheet->exists())
    {
        styleSheet = new QFile(":files/styles/style-indoor.css");
    }
    if (styleSheet->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString style = QString(styleSheet->readAll());
        style.replace("ICONDIR", QCoreApplication::applicationDirPath()+ "files/styles/");
        qApp->setStyleSheet(style);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("QGroundControl did lot load a new style"));
        msgBox.setInformativeText(tr("Stylesheet file %1 was not readable").arg(styleFileName));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    delete styleSheet;
}

/** @brief Shows an info message as popup or as widget */
void MainWindow::showInfoMessage(const QString &title, const QString &message)
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(title);
    msgBox.setInformativeText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::selectStylesheet()
{
}

void MainWindow::loadNativeStyle()
{
    loadStyle(GCS_MAINWINDOW_STYLE_NATIVE);
}

void MainWindow::loadIndoorStyle()
{
    loadStyle(GCS_MAINWINDOW_STYLE_INDOOR);
}

void MainWindow::loadOutdoorStyle()
{
    loadStyle(GCS_MAINWINDOW_STYLE_OUTDOOR);
}

void MainWindow::comPortOpenCloseButtonClick()
{
    if(m_SerialConfigWindow->SerialSettingsWindow::port->isOpen())
    {
        m_SerialConfigWindow->SerialSettingsWindow::port->close();
        ui->comOpenCloseButton->setText("Open");
    }
    else
    {
        m_SerialConfigWindow->SerialSettingsWindow::port->setPortName(m_SerialConfigWindow->port_ui->portBox->currentText());
        m_SerialConfigWindow->SerialSettingsWindow::port->open(QIODevice::ReadWrite);
        ui->comOpenCloseButton->setText("Close");
    }
    ui->comPortStatusLued->turnOn(m_SerialConfigWindow->SerialSettingsWindow::port->isOpen());
}

void MainWindow::onComSettingTriggered()
{
    m_SerialConfigWindow->show();

}

void MainWindow::onComOpenCloseTriggered()
{
  m_port = m_SerialConfigWindow->port;
  if(m_port->isOpen())
  {
//      m_port->close();
      ui->actionConnect->setText("Open");
  }
  else
  {
//      m_port->open(QIODevice::ReadWrite);
      ui->actionConnect->setText("Close");
  }

}
