// Fill out your copyright notice in the Description page of Project Settings.

#include "SkeletalGameEntity.h"

// Sets default values for this character's properties
ASkeletalGameEntity::ASkeletalGameEntity()
{
    // Skeletal mesh
    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    AddOwnedComponent(Mesh);

    // Navigation
    NavigatorComponent = CreateDefaultSubobject<UNavigatorComponent>(TEXT("NavigationComponent"));
    AddOwnedComponent(NavigatorComponent);

    // Dialog
    DialogComponent = CreateDefaultSubobject<UDialogComponent>(TEXT("DialogComponent"));
    if (IsValid(DialogAsset))
    {
        DialogComponent->DialogAsset = DialogAsset;
    }
    AddOwnedComponent(DialogComponent);
}

// Called when the game starts or when spawned
void ASkeletalGameEntity::BeginPlay()
{
    //Something
}

// Called every frame
void ASkeletalGameEntity::Tick(float DeltaTime)
{
    // Every tick
}
