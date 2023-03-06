// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Quest.h"

#include "QuestComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UQuestComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
    UQuestComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<TObjectPtr<UQuest>> Quests;

	UFUNCTION(BlueprintCallable)
    UQuest* GetQuest(EQuestList QuestId);
};
