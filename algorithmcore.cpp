#include "algorithmcore.h"

AlgorithmCore *AlgorithmCore::instance = nullptr;

AlgorithmCore::AlgorithmCore(QObject *parent)
    : QObject{parent}
{
    dataHandler = DataHandler::getInstance();
    initConnections();
}

void AlgorithmCore::initConnections()
{
    connect(dataHandler, &DataHandler::ballastResistorListProcessed, this, &AlgorithmCore::startAlgorithm);
}

AlgorithmCore *AlgorithmCore::getInstance()
{
    if (AlgorithmCore::instance == nullptr) AlgorithmCore::instance = new AlgorithmCore();
    return AlgorithmCore::instance;
}

void AlgorithmCore::greedyAlgorihtm(double load, QList<Resistor *> list)
{
    if (list.isEmpty()) return;

    double smallestConsumption = list.last()->getConsumption();

    for(auto item: list)
    {
        if (item->getConsumption() <= load || abs(load-item->getConsumption()) <= smallestConsumption/2.0)
        {
            item->activate();
            load -= item->getConsumption();
        }
    }
}

void AlgorithmCore::startAlgorithm()
{
    auto list = dataHandler->getBallastResisterList();
    qDebug() << "AlgorithmCore: start greedyAlgorihtm. List size =" << list.size();

    greedyAlgorihtm(dataHandler->getLoad(), list);

    qDebug() << "AlgorithmCore: complete greedyAlgorihtm";
    emit algorithComplete();
}
