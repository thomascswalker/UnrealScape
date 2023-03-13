// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Quest.h"
#include "QuestComponent.h"
#include "USPlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"

#include "QuestFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API UQuestFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    static UQuestComponent* GetQuestComponent(const UObject* Context);

    UFUNCTION(BlueprintPure, Category = "Quests", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static UQuest* GetQuest(const UObject* Context, EQuestList Id);

    UFUNCTION(BlueprintCallable, Category = "Quests",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool IsQuestStarted(const UObject* Context, EQuestList Id);

    UFUNCTION(BlueprintCallable, Category = "Quests",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool IsQuestComplete(const UObject* Context, EQuestList Id);

    UFUNCTION(BlueprintCallable, Category = "Quests", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void StartQuest(const UObject* Context, EQuestList Id);

    UFUNCTION(BlueprintCallable, Category = "Quests", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void CompleteQuest(const UObject* Context, EQuestList Id);

    UFUNCTION(BlueprintPure, Category = "Quests", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static int GetQuestStep(const UObject* Context, EQuestList Id);

    UFUNCTION(BlueprintCallable, Category = "Quests", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void SetQuestStep(const UObject* Context, EQuestList Id, int Step);
};
