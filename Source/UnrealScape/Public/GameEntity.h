// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactive.h"
#include "Kismet/GameplayStatics.h"
#include "USUtils.h"

#include "GameEntity.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionComplete);

UCLASS()
class UNREALSCAPE_API AGameEntity : public AActor, public IInteractive
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    FString Name;

    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    EEntityType Type;

    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    TArray<FOption> Options;

public:
    // Sets default values for this actor's properties
    AGameEntity();

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    float InteractDistance = 75.f;  // Slightly more than less than one straight tile distance

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintCallable, Category = "Event Dispatchers")
    FInteractionComplete InteractionComplete;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(const FOption& Option) override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure)
    FVector GetFloor() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure)
    APlayerController* GetPlayer() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure)
    FString GetName() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure)
    EEntityType GetType() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure)
    TArray<FOption> GetOptions() override;

    UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="OldName,NewName"))
    void SetOptionName(const FString& OldName, const FString& NewName);
};
