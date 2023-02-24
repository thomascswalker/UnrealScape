// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogInterpreterComponent.h"

// Sets default values for this component's properties
UDialogInterpreterComponent::UDialogInterpreterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDialogInterpreterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDialogInterpreterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDialogInterpreterComponent::Start(UDialogAsset* Dialog)
{
    if (!IsValid(Dialog))
    {
        WARNING(TEXT("Dialog not found on NPC!"));
		return;
	}
    Dialog->Construct();
    DialogStarted.Broadcast(Dialog);
}