// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Grid.h"
#include "../Tile.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

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

    AGrid* GetCurrentGrid();
    FTileInfo& GetCurrentTile();
    FTileInfo& GetTileUnderCursor();
};
