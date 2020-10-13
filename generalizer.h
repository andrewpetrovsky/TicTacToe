#ifndef GENERALIZER_H
#define GENERALIZER_H

#include <QObject>

class Generalizer : public QObject
{
    Q_OBJECT
public:
    explicit Generalizer(QList<QPair<QVector<float>, float>> trainingExamples,QObject *parent = nullptr);
    float GetNonFinalBoardScore(QVector<float> weightVector, QVector<float> featureVector);
    QVector<float> UpdateWeightVector(QVector<float> weightVector,float alpha);
private:
    QList<QPair<QVector<float>, float>> _trainingExamples;



};

#endif // GENERALIZER_H
