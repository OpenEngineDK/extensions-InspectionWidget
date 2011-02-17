// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_INSPECTION_WIDGET_H_
#define _OE_INSPECTION_WIDGET_H_

#include <QtGui>
#include <Utils/IInspector.h>
#include <string>
#include <Logging/Logger.h>
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Utils/Timer.h>

namespace OpenEngine {
namespace Display {

using namespace Utils::Inspection;
using namespace Core;
using namespace Utils;

class ValueObject : public QObject {
    Q_OBJECT
    public:
    virtual void Refresh() = 0;
};

class ActionValueObject : public ValueObject {
    Q_OBJECT
    ActionValue* val;
public:
    ActionValueObject(ActionValue* val) : val(val) {}
    void Refresh() {}
public slots:
    void call(bool a) {
        val->Call();
    }
};

class RValueObjectFloat : public ValueObject {
    Q_OBJECT
    RValue<float>* val;
public:
    RValueObjectFloat(RValue<float>* val) : val(val) {}
    
    void Refresh() {
        emit valueChanged(val->Get() );
    }

signals:
    void valueChanged(double newValue);
};

class RWValueObjectFloat : public ValueObject {
    Q_OBJECT
    RWValue<float>* val;
    float lastVal;
public:
    float scale;
    RWValueObjectFloat(RWValue<float> *val) : val(val) {  }
    int value() const { 
        return val->Get() * scale;
    }

    void Refresh() {
        if (val->Get() == lastVal) return;
        emit valueChanged( val->Get() );
        emit valueChangedInt( val->Get() * scale);
        emit valueChangedString(QString::number(val->Get()));
        lastVal = val->Get();
    }

 public slots:
    void setValue(int value) { 
        val->Set(value/scale);
        Refresh();
    }
    void setStringValue(const QString& str) {
        val->Set(str.toFloat());
    }

signals:
    void valueChanged(double newValue);
    void valueChangedInt(int newValue);
    void valueChangedString(const QString&);
};

class RWValueObjectUInt : public ValueObject {
    Q_OBJECT
    RWValue<unsigned int>* val;
 public:
    RWValueObjectUInt(RWValue<unsigned int> *val) : val(val) {  }
    int value() const { 
        return val->Get(); 
    }

    void Refresh() {
        emit valueChanged( val->Get() );
    }

 public slots:
    void setValue(int value) { 
        val->Set(value);
        Refresh();
    }

signals:
    void valueChanged(int newValue);
 };


/**
 * Short description.
 *
 * @class InspectionWidget InspectionWidget.h ons/InspectionWidget/Display/InspectionWidget.h
 */
class InspectionWidget : public QWidget 
                       , public IListener<Core::ProcessEventArg> {
private:
    Timer timer;
    list<ValueObject*> objects;
public:
    InspectionWidget(std::string title, Utils::Inspection::ValueList vl);
    void Handle(Core::ProcessEventArg arg);
};

} // NS Display
} // NS OpenEngine

#endif // _OE_INSPECTION_WIDGET_H_
