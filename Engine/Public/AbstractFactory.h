#pragma once

#include "Graphic_Device.h"
#include "Timer_Manager.h"

class AbstractFactory
{
public:
	template<typename T>
	static T* Create(T*& Instance)
	{
		Instance = new T;

		if (Instance == nullptr)
			return nullptr;

		return Instance;
	}

};

