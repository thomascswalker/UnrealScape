// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// TODO: Convert this to search the quests directory and programmatically generate each quest
	static ConstructorHelpers::FClassFinder<UQuest> TutorialIsland(TEXT("/Game/Blueprints/Quests/BP_TutorialIsland"));
    UQuest* TutorialIslandQuest = NewObject<UQuest>(this, TutorialIsland.Class, FName("TutorialIsland"));
    Quests.Add(TutorialIslandQuest);
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UQuest* UQuestComponent::GetQuest(EQuestList QuestId)
{
    for (UQuest* Quest : Quests)
    {
        if (Quest->QuestId == QuestId)
        {
            return Quest;
		}
	}
    return nullptr;
}

