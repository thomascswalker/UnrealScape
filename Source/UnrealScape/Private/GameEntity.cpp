// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntity.h"

// Sets default values
AGameEntity::AGameEntity()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameEntity::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AGameEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGameEntity::Interact_Implementation(AActor* Actor)
{
    FString Message = FString::Printf(L"%s interacting with %s", *Actor->GetName(), *GetName());
    INFO(Message);
}
