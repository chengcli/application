//! @file global.cpp

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#include "cantera/base/FactoryBase.h"
#include "application.h"
#include "cantera/base/AnyMap.h"

namespace Global
{

static Application* app()
{
    return Application::Instance();
}

void terminate()
{
    Application::ApplicationDestroy();
}
