#include "generalizer.h"

Generalizer::Generalizer(QList<QPair<QVector<float>, float>> trainingExamples, QObject *parent) : QObject(parent)
{

    _trainingExamples = trainingExamples;
}

float Generalizer::GetNonFinalBoardScore(QVector<float> weightVector, QVector<float> featureVector)
{
    if(weightVector.size() != featureVector.size())
        return -1;
    float temp = 0;
    for (int i = 0; i < weightVector.size(); ++i)
        temp += weightVector[i]*featureVector[i];

    return temp;
}

QVector<float> Generalizer::UpdateWeightVector(QVector<float> weightVector, float alpha = 0.4)
{
    //Weight Vector is the calculated emphesis on each players position
    QVector<float> updatedWeightVector = weightVector;
    for (int i = 0; i < _trainingExamples.count(); ++i)
    {
        //_TrainingExamples(first)/featureVector is how the board is described
        // i.e the number of same spots in row taken
        QVector<float> tempWeightVector = _trainingExamples.at(i).first;
        //Dot product the two vectors
        float tempScore = GetNonFinalBoardScore(weightVector,tempWeightVector);
        float tempValue = alpha * (_trainingExamples.at(i).second - tempScore);
        for (int j = 0; j < tempWeightVector.count(); ++j)
        {
            tempWeightVector[j] *= tempValue;
            updatedWeightVector[j] += tempWeightVector[j];
        }


    }
    return updatedWeightVector;
}
