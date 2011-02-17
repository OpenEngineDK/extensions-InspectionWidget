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
        if (ActionValue* val = dynamic_cast<ActionValue*>(*itr)) {
            QString name = QString::fromStdString(val->name);
            QPushButton* btn = new QPushButton(name);
            
            ActionValueObject* obj = new ActionValueObject(val);
            QObject::connect(btn, SIGNAL(clicked(bool)),
                             obj, SLOT(call(bool)));

            objects.push_back(obj);

            layout->addRow(name, btn);
            
        } else if (RValue<float>* val = dynamic_cast<RValue<float> *>(*itr)) {
            QLabel *l = new QLabel();

            RValueObjectFloat* obj = new RValueObjectFloat(val);

            QObject::connect(obj, SIGNAL(valueChanged(double)),
                             l, SLOT(setNum(double)));

            objects.push_back(obj);

            QString str = QString::fromStdString(val->name);
            
            obj->Refresh();

            layout->addRow(str, l);
            
        } else if (RWValue<float> *val = dynamic_cast<RWValue<float> *>(*itr)) {
            QAbstractSlider *w = new QSlider();
            QLineEdit *le = new QLineEdit();
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

            QObject::connect(obj, SIGNAL(valueChangedString(const QString&)),
                             le, SLOT(setText(const QString&)));

            QObject::connect(le, SIGNAL(textEdited(const QString&)),
                             obj, SLOT(setStringValue(const QString&)));

            objects.push_back(obj);

            QString str = QString::fromStdString(val->name);

            box->addWidget(le, Qt::AlignLeft);
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
        if (timer.GetElapsedTime().AsInt32() > 400000) {
            for (list<ValueObject*>::iterator itr = objects.begin();
                 itr != objects.end();
                 itr++) {
                (*itr)->Refresh();
            }



            timer.Reset();
        }
    }

}
}
