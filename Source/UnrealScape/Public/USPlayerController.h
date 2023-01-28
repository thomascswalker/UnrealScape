// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Grid.h"
#include "../Tile.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "USCharacter.h"

#include "USPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API AUSPlayerController : public APlayerController
{
    GENERATED_BODY()

    float MovementSpeed = 1.f;
    USplineComponent* Spline;
    float GoalThreshold = 100.f;
    FVector Goal;
    FVector NextPoint;
    float Length;
    float CurrentTime = 0.f;
    float DistanceThreshold = 50.f;
    float DistanceBetweenPoints = 100.f;

public:
    AUSPlayerController();

    // Overrides
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // Methods
    UFUNCTION(BlueprintCallable, Category = "Utility")
    bool LineTraceUnderMouseCursor(FHitResult& HitResult);
};
