// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameTaskComponent.h"
#include "Grid.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TaskExecutor.h"
#include "Tile.h"
#include "USCharacter.h"
#include "GameEntity.h"

#include "USPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API AUSPlayerController : public APlayerController, public ITaskExecutor
{
    GENERATED_BODY()

public:
    AUSPlayerController();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    UGameTaskComponent* GameTaskComponent;

    AGameEntity* TargetEntity;
    FDelegateHandle TargetEntityInteractHandle;

    // Overrides
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // Methods
    UFUNCTION(BlueprintCallable, Category = "Utility")
    bool LineTraceUnderMouseCursor(FHitResult& HitResult, ECollisionChannel CollisionChannel);

    UNavigatorComponent* GetNavigatorComponent() override;

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void OnLeftClick();

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void Move(const FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void MoveAndInteract(const FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void InteractionComplete();

    UFUNCTION(BlueprintCallable, Category = "Visual")
    void UpdateFloorVisibility();
};
