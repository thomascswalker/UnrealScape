// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Tile.h"
#include "USPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API AUSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AUSPlayerController();

    // Overrides
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // Methods
    void OnLeftClick();
    bool LineTraceUnderMouseCursor(FHitResult& HitResult);


    // Attributes
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pathfinding")
    ATile* CurrentTile;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pathfinding")
    ATile* TargetTile;
};
