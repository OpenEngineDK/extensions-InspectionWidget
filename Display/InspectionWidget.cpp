#include "InspectionWidget.h"
#include <Logging/Logger.h>

using namespace std;
using namespace OpenEngine::Utils::Inspection;

namespace OpenEngine {
namespace Display {
    



InspectionWidget::InspectionWidget(string title, ValueList vl) {
    QFormLayout* layout = new QFormLayout();
    setLayout(layout);

    for (ValueList::iterator itr = vl.begin();
         itr != vl.end();
         itr++) {
        if (RWValue<float> *val = dynamic_cast<RWValue<float> *>(*itr)) {
            QAbstractSlider *w = new QSlider();
            QLabel *l = new QLabel();
            float scale = 10.0;
            if (val->properties.count(STEP)) {
                scale = 1/(val->properties[STEP]);
            }
            if (val->properties.count(MIN)) {
                w->setMinimum(val->properties[MIN]*scale);
            }
            if (val->properties.count(MAX)) {
                w->setMaximum(val->properties[MAX]*scale);
            }

            w->setOrientation(Qt::Horizontal);

            QHBoxLayout* box = new QHBoxLayout();

            RWValueObjectFloat* obj = new RWValueObjectFloat(val);
            obj->scale = scale;
            QObject::connect(w, SIGNAL(valueChanged(int)),
                             obj, SLOT(setValue(int)));

            QObject::connect(obj, SIGNAL(valueChanged(double)),
                             l, SLOT(setNum(double)));

            objects.push_back(obj);

            QString str = QString::fromStdString(val->name);

            box->addWidget(l,Qt::AlignLeft);
            box->addWidget(w,Qt::AlignRight);
            
            obj->Refresh();

            layout->addRow(str, box);

        } else if (RWValue<unsigned int> *val = dynamic_cast<RWValue<unsigned int> *>(*itr)) {
            //QAbstractSlider *w = new QDial();
            QAbstractSlider *w = new QSlider();
            QLabel *l = new QLabel();
            if (val->properties.count(MIN)) {
                w->setMinimum(val->properties[MIN]);
            }
            if (val->properties.count(MAX)) {
                w->setMaximum(val->properties[MAX]);
            }

            w->setOrientation(Qt::Horizontal);

            QHBoxLayout* box = new QHBoxLayout();

            RWValueObjectUInt* obj = new RWValueObjectUInt(val);

            QObject::connect(w, SIGNAL(valueChanged(int)),
                             obj, SLOT(setValue(int)));

            QObject::connect(obj, SIGNAL(valueChanged(int)),
                             l, SLOT(setNum(int)));

            objects.push_back(obj);

            QString str = QString::fromStdString(val->name);

            box->addWidget(l,Qt::AlignLeft);
            box->addWidget(w,Qt::AlignRight);
            
            obj->Refresh();

            layout->addRow(str, box);
        }
    }

    timer.Start();
}

    void InspectionWidget::Handle(Core::ProcessEventArg arg) {
        if (timer.GetElapsedTime().sec > 1) {
            for (list<RWValueObject*>::iterator itr = objects.begin();
                 itr != objects.end();
                 itr++) {
                (*itr)->Refresh();
            }



            timer.Reset();
        }
    }

}
}
