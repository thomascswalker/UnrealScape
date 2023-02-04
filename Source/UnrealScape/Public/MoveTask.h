// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTask.h"
#include "MoveTask.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API UMoveTask : public UGameTask
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Game Tasks", Meta = (ExposeOnSpawn = "true"))
    FVector Location;

    UNavigatorComponent* Navigator;

    UMoveTask();
    void Execute() override;
    void Bind() override;
    void Unbind() override;

    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    void OnStarted() override;
    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    void OnCompleted() override;
    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    void OnCancelled() override;
};
