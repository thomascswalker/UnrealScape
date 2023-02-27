// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigatorComponent.h"

#include "USCharacter.generated.h"

UCLASS()
class UNREALSCAPE_API AUSCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AUSCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Properties
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    TObjectPtr<UNavigatorComponent> NavigatorComponent;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
    float CameraRotationSpeed = 2.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
    float CameraZoomSpeed = 10.f;

    // Functions
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnCameraRotateRight(float Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnCameraRotateUp(float Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnCameraZoom(float Delta);
};