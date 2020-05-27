#pragma once
#include "All.h"
#include <dwrite.h>
#include <d2d1.h>

extern IDWriteFactory* g_dwriteFactory;

template <typename InterfaceType>
InterfaceType* SafeAcquire(InterfaceType* newObject){
	if (newObject != NULL)
		newObject->AddRef();
	return newObject;
}