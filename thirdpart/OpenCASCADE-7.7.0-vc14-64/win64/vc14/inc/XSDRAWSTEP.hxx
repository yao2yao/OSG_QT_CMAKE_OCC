// Created on: 1996-01-12
// Created by: Christian CAILLET
// Copyright (c) 1996-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _XSDRAWSTEP_HeaderFile
#define _XSDRAWSTEP_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>

#include <Draw_Interpretor.hxx>


//! XSDRAW for STEP AP214 and AP203
class XSDRAWSTEP 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT static void Init();
  
  //! Inits commands to access product data and to write shapes
  Standard_EXPORT static void InitCommands (Draw_Interpretor& theCommands);




protected:





private:





};







#endif // _XSDRAWSTEP_HeaderFile
