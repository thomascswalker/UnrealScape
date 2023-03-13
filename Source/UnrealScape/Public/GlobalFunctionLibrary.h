// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Chatbox.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "USUtils.h"

#include "GlobalFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API UGlobalFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Global",
              meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static UChatbox* GetChatbox(UObject* Context);

    UFUNCTION(BlueprintCallable, Category = "Global", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AddChatboxMessage(UObject* Context, const FText Text);

    UFUNCTION(BlueprintCallable, Category = "Global", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static FVector GetPlayerLocation(UObject* Context);
};
