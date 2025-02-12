#include "data_source.h"

#include <QRandomGenerator>
#include <QElapsedTimer>
#include <QXYSeries>
#include <QtMath>
#include <QTime>

data_source::data_source(QObject* parent)
	: QObject(parent)
{
	generateData(0, 5, 1024);
}

void data_source::generateData(int type, int rowCount, int colCount)
{
	// Remove previous data
	m_data.clear();

	// Append the new data depending on the type
	for(int i(0); i < rowCount; i++)
	{
		QList<QPointF> points;
		points.reserve(colCount);
		for(int j(0); j < colCount; j++)
		{
			qreal x(0);
			qreal y(0);
			switch(type)
			{
			case 0:
				// data with sin + random component
				y = qSin(M_PI / 50 * j) + 0.5 +
				    QRandomGenerator::global()->generateDouble();
				x = j;
				break;
			case 1:
				// linear data
				x = j;
				y = (qreal)i / 10;
				break;
			default:
				// unknown, do nothing
				break;
			}
			points.append(QPointF(x, y));
		}
		m_data.append(points);
	}
}

void data_source::update(QAbstractSeries* series)
{
	if(series)
	{
		QXYSeries* xySeries = static_cast<QXYSeries*>(series);

		static QElapsedTimer time;
		static bool first = true;
		if(first)
		{
			time.start();
			first = false;
		}
		double key = time.elapsed() / 1000.0;
		static double lastPointKey = 0;

		if(key - lastPointKey > 0.002)
		{
			QPointF point(key,
				      qSin(key) + std::rand() / static_cast<double>(RAND_MAX) * 1 *
							  qSin(key / 0.3843));
			lastPointKey = key;
			xySeries->append(point);
		}
	}
}
