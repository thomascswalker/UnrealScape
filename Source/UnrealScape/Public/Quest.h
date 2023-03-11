// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "USUtils.h"

#include "Quest.generated.h"

UENUM(BlueprintType)
enum class EQuestList : uint8
{
    TutorialIsland
};

UENUM(BlueprintType)
enum class EQuestStepType : uint8
{
    Start,
    Default,
    End
};

USTRUCT(BlueprintType)
struct FQuestStep
{
public:
    GENERATED_BODY();

    UPROPERTY(BlueprintReadWrite)
    FText JournalText;

    UPROPERTY(BlueprintReadWrite)
    int NextStep;

    UPROPERTY(BlueprintReadWrite)
    EQuestStepType Type;
};

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable, BlueprintType, Meta = (ShowWorldContextPin))
class UNREALSCAPE_API UQuest : public UObject
{
    GENERATED_BODY()

public:
    // Properties
    UPROPERTY(EditDefaultsOnly)
    EQuestList QuestId;

    UPROPERTY(EditDefaultsOnly)
    FString DisplayName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int CurrentStep;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool bComplete = false;

    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestStarted, UQuest*, Quest);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FQuestStarted QuestStarted;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestCompleted, UQuest*, Quest);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FQuestCompleted QuestCompleted;

    // Functions
    UFUNCTION(BlueprintCallable, BlueprintPure)
    virtual UWorld* GetWorld() const override;

    void Start();
    void Complete();

    UFUNCTION(BlueprintNativeEvent)
    void Update(int Step);

    UFUNCTION(BlueprintNativeEvent)
    void Reward();
};
