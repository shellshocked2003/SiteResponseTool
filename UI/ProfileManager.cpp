#include "ProfileManager.h"

ProfileManager::ProfileManager(QWidget *parent) : QDialog(parent)
{

}

ProfileManager::ProfileManager(QTabWidget *tab,PostProcessor *postProcessort,QWidget *parent) :
    QDialog(parent), m_tab(tab),postProcessor(postProcessort)
{

    connect(m_tab, SIGNAL(tabBarClicked(int)), this, SLOT(onTabBarClicked(int)));

    // pga view
    pgaHtmlView = new QWebEngineView(this);
    if(!QFileInfo(pgaHtmlName).exists())
    {
        // get file paths
        QFileInfo htmlInfo(pgaHtmlName);
        //QString dir = htmlInfo.path();
        QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/pga-template.html");
        QString newPath = QDir(rootDir).filePath("resources/ui/Profile/pga.html");
        QFile::copy(tmpPath, newPath);
    }
    pgaHtmlView->load(QUrl::fromLocalFile(QFileInfo(pgaHtmlName).absoluteFilePath()));
    pgaHtmlView->show();
    m_tab->addTab(pgaHtmlView,"PGA(g)");


    // gamma max view
    gammaHtmlView = new QWebEngineView(this);
    if(!QFileInfo(gammaHtmlName).exists())
    {
        // get file paths
        QFileInfo htmlInfo(gammaHtmlName);
        //QString dir = htmlInfo.path();
        QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/gamma-template.html");
        QString newPath = QDir(rootDir).filePath("resources/ui/Profile/gamma.html");
        QFile::copy(tmpPath, newPath);
    }
    gammaHtmlView->load(QUrl::fromLocalFile(QFileInfo(gammaHtmlName).absoluteFilePath()));
    gammaHtmlView->show();
    m_tab->addTab(gammaHtmlView,"\u03B3max(%)");





    // disp max view
    dispHtmlView = new QWebEngineView(this);
    if(!QFileInfo(dispHtmlName).exists())
    {
        // get file paths
        QFileInfo htmlInfo(dispHtmlName);
        //QString dir = htmlInfo.path();
        QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/disp-template.html");
        QString newPath = QDir(rootDir).filePath("resources/ui/Profile/disp.html");
        QFile::copy(tmpPath, newPath);
    }
    dispHtmlView->load(QUrl::fromLocalFile(QFileInfo(dispHtmlName).absoluteFilePath()));
    dispHtmlView->show();
    m_tab->addTab(dispHtmlView,"maxDisp(m)");

    // ru max view
    ruHtmlView = new QWebEngineView(this);
    if(!QFileInfo(ruHtmlName).exists())
    {
        // get file paths
        QFileInfo htmlInfo(dispHtmlName);
        //QString dir = htmlInfo.path();
        QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/ru-template.html");
        QString newPath = QDir(rootDir).filePath("resources/ui/Profile/ru.html");
        QFile::copy(tmpPath, newPath);
    }
    ruHtmlView->load(QUrl::fromLocalFile(QFileInfo(ruHtmlName).absoluteFilePath()));
    ruHtmlView->show();
    m_tab->addTab(ruHtmlView,"maxRu");


    // rupwp max view
    rupwpHtmlView = new QWebEngineView(this);
    if(!QFileInfo(rupwpHtmlName).exists())
    {
        // get file paths
        QFileInfo htmlInfo(rupwpHtmlName);
        //QString dir = htmlInfo.path();
        QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/rupwp-template.html");
        QString newPath = QDir(rootDir).filePath("resources/ui/Profile/rupwp.html");
        QFile::copy(tmpPath, newPath);
    }
    rupwpHtmlView->load(QUrl::fromLocalFile(QFileInfo(rupwpHtmlName).absoluteFilePath()));
    rupwpHtmlView->show();
    m_tab->addTab(rupwpHtmlView,"maxRupwp");


    m_tab->setTabPosition(QTabWidget::East);

}

void ProfileManager::updatePostProcessor(PostProcessor *postProcessort)
{
    postProcessor = postProcessort;
}

void ProfileManager::onTabBarClicked(int ind)
{
    qDebug() << "Tab " << ind << " clicked.";
    // reload all views in tab
    //pgaHtmlView->reload();
    //gammaHtmlView->reload();
    //pgaHtmlView->load(QUrl::fromLocalFile(QFileInfo(pgaHtmlName).absoluteFilePath()));
    //gammaHtmlView->load(QUrl::fromLocalFile(QFileInfo(gammaHtmlName).absoluteFilePath()));
}

void ProfileManager::onPostProcessorUpdated()
{
    updatePGAHtml();
    updateRuHtml();
    updateGammaHtml();
    updaterupwpHtml();
    updateDispHtml();



    // reload all views in tab
    pgaHtmlView->reload();
    pgaHtmlView->show();
    gammaHtmlView->reload();
    gammaHtmlView->show();
    rupwpHtmlView->reload();
    rupwpHtmlView->show();
    dispHtmlView->reload();
    dispHtmlView->show();
    ruHtmlView->reload();
    ruHtmlView->show();



}

void ProfileManager::updatePGAHtml()
{
    // get file paths
    QFileInfo htmlInfo(pgaHtmlName);
    //QString dir = htmlInfo.path();
    QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/pga-template.html");
    QString newPath = QDir(rootDir).filePath("resources/ui/Profile/pga.html");
    QFile::remove(newPath);

    // read template file into string
    QFile file(tmpPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    QString text = QString(t);
    file.close();


    //QString insertedString = loadGMtoString();
    QString insertedString;
    QTextStream stream(&insertedString);
    QVector<double> depths =  postProcessor->getDepths();
    QVector<double> pga =  postProcessor->getPga();

    stream << "xd = ['Depth'";
    for (int i=0;i<depths.size();i++)
        stream << ", " << depths[i];
    stream << "]; \n";
    stream << "yd = ['PGA'";
    for (int i=0;i<pga.size();i++)
        stream << ", " << pga[i];
    stream << "]; \n";
    text.replace(QString("//UPDATEPOINT"), insertedString);

    // write to index.html
    QFile newfile(newPath);
    newfile.open(QIODevice::WriteOnly | QIODevice::Text);
    newfile.write(text.toUtf8());
    newfile.close();


}

void ProfileManager::updateGammaHtml()
{
    // get file paths
    QFileInfo htmlInfo(gammaHtmlName);
    //QString dir = htmlInfo.path();
    QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/gamma-template.html");
    QString newPath = QDir(rootDir).filePath("resources/ui/Profile/gamma.html");
    QFile::remove(newPath);

    // read template file into string
    QFile file(tmpPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    QString text = QString(t);
    file.close();


    //QString insertedString = loadGMtoString();
    QString insertedString;
    QTextStream stream(&insertedString);
    QVector<double> depths =  postProcessor->getRuDepths();
    QVector<double> gamma =  postProcessor->getGamma();

    stream << "xd = ['Depth'";
    for (int i=0;i<depths.size();i++)
        stream << ", " << depths[i];
    stream << "]; \n";
    stream << "yd = ['&gamma;',NaN";
    for (int i=0;i<gamma.size();i++)
        stream << ", " << gamma[i];
    stream << ",NaN]; \n";
    text.replace(QString("//UPDATEPOINT"), insertedString);

    // write to index.html
    QFile newfile(newPath);
    newfile.open(QIODevice::WriteOnly | QIODevice::Text);
    newfile.write(text.toUtf8());
    newfile.close();


}


void ProfileManager::updaterupwpHtml()
{
    // get file paths
    QFileInfo htmlInfo(rupwpHtmlName);
    //QString dir = htmlInfo.path();
    QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/rupwp-template.html");
    QString newPath = QDir(rootDir).filePath("resources/ui/Profile/rupwp.html");
    QFile::remove(newPath);

    // read template file into string
    QFile file(tmpPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    QString text = QString(t);
    file.close();


    //QString insertedString = loadGMtoString();
    QString insertedString;
    QTextStream stream(&insertedString);
    QVector<double> depths =  postProcessor->getRuDepths();
    QVector<double> rupwp =  postProcessor->getRupwp();

    stream << "xd = ['Depth'";
    for (int i=0;i<depths.size();i++)
        stream << ", " << depths[i];
    stream << "]; \n";
    stream << "yd = ['Rupwp',NaN";
    for (int i=0;i<rupwp.size();i++)
        stream << ", " << rupwp[i]/100.0;
    stream << ",NaN]; \n";
    text.replace(QString("//UPDATEPOINT"), insertedString);

    // write to index.html
    QFile newfile(newPath);
    newfile.open(QIODevice::WriteOnly | QIODevice::Text);
    newfile.write(text.toUtf8());
    newfile.close();


}



void ProfileManager::updateDispHtml()
{
    // get file paths
    QFileInfo htmlInfo(gammaHtmlName);
    //QString dir = htmlInfo.path();
    QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/disp-template.html");
    QString newPath = QDir(rootDir).filePath("resources/ui/Profile/disp.html");
    QFile::remove(newPath);

    // read template file into string
    QFile file(tmpPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    QString text = QString(t);
    file.close();


    //QString insertedString = loadGMtoString();
    QString insertedString;
    QTextStream stream(&insertedString);
    QVector<double> depths =  postProcessor->getDepths();
    QVector<double> disp =  postProcessor->getDisp();

    stream << "xd = ['Depth'";
    for (int i=0;i<depths.size();i++)
        stream << ", " << depths[i];
    stream << "]; \n";
    stream << "yd = ['maxDisp'";
    for (int i=0;i<disp.size();i++)
        stream << ", " << disp[i];
    stream << "]; \n";
    text.replace(QString("//UPDATEPOINT"), insertedString);

    // write to index.html
    QFile newfile(newPath);
    newfile.open(QIODevice::WriteOnly | QIODevice::Text);
    newfile.write(text.toUtf8());
    newfile.close();


}



void ProfileManager::updateRuHtml()
{
    // get file paths
    QFileInfo htmlInfo(gammaHtmlName);
    //QString dir = htmlInfo.path();
    QString tmpPath = QDir(rootDir).filePath("resources/ui/Profile/ru-template.html");
    QString newPath = QDir(rootDir).filePath("resources/ui/Profile/ru.html");
    QFile::remove(newPath);


    // read template file into string
    QFile file(tmpPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    QString text = QString(t);
    file.close();


    //QString insertedString = loadGMtoString();
    QString insertedString;
    QTextStream stream(&insertedString);
    QVector<double> depths =  postProcessor->getRuDepths();
    QVector<double> ru =  postProcessor->getRu();

    stream << "xd = ['Depth'";
    for (int i=0;i<depths.size();i++)
        stream << ", " << depths[i];
    stream << "]; \n";
    stream << "yd = ['maxRu',NaN";
    for (int i=0;i<ru.size();i++)
        stream << ", " << ru[i];
    stream << ",NaN]; \n";
    text.replace(QString("//UPDATEPOINT"), insertedString);

    // write to index.html
    QFile newfile(newPath);
    newfile.open(QIODevice::WriteOnly | QIODevice::Text);
    newfile.write(text.toUtf8());
    newfile.close();


}
