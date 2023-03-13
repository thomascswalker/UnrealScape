// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HintComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "USPlayerController.h"

#include "HintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API UHintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Hints", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static UHintComponent* GetHintComponent(const UObject* Context);

    UFUNCTION(BlueprintCallable, Category = "Hints", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void CreateHint(const UObject* Context, FVector Location, bool bClearHints);

    UFUNCTION(BlueprintCallable, Category = "Hints", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void RemoveHint(const UObject* Context, int Index);

    UFUNCTION(BlueprintCallable, Category = "Hints", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void ClearHints(const UObject* Context);
};
