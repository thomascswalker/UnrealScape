// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"
#include "Tile.generated.h"

UCLASS()
class RS_MOVEMENT_CPP_API ATile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UStaticMeshComponent* StaticMeshComponent;
    UTextRenderComponent* TextRenderComponent;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    bool IsWalkable();

    void SetText(FText& Text);
};
