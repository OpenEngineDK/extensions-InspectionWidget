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

class RWValueObjectUInt : public QObject {
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

private:
    int m_value;
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
    list<RWValueObjectUInt*> objects;
public:
    InspectionWidget(std::string title, Utils::Inspection::ValueList vl);
    void Handle(Core::ProcessEventArg arg);
};

} // NS Display
} // NS OpenEngine

#endif // _OE_INSPECTION_WIDGET_H_
