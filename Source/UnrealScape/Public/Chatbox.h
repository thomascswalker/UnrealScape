// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Chatbox.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API UChatbox : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void AddItem(const FText& Text);
};
