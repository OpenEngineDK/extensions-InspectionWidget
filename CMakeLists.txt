QT4_WRAP_CPP(MOC_SOURCE
  Display/InspectionWidget.h

)

# Create the extension library
ADD_LIBRARY(Extensions_InspectionWidget
  Display/InspectionWidget.h
  Display/InspectionWidget.cpp
  ${MOC_SOURCE}
)
