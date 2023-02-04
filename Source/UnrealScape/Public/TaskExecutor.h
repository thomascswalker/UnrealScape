// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NavigatorComponent.h"

#include "TaskExecutor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTaskExecutor : public UInterface
{
	GENERATED_BODY()
};

class UNREALSCAPE_API ITaskExecutor
{
	GENERATED_BODY()

	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual UNavigatorComponent* GetNavigatorComponent();
};
