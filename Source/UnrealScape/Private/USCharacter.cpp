// Fill out your copyright notice in the Description page of Project Settings.

#include "USCharacter.h"

// Sets default values
AUSCharacter::AUSCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Spring Arm
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    SpringArmComponent->TargetArmLength = 1200.f;
    SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->bInheritPitch = false;
    SpringArmComponent->bInheritYaw = false;
    SpringArmComponent->bInheritRoll = false;
    SpringArmComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, -45.f, 0.f)));
    SpringArmComponent->SetupAttachment(RootComponent);

    // Camera
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    // Navigation
    NavigatorComponent = CreateDefaultSubobject<UNavigatorComponent>(TEXT("Navigator"));
    AddOwnedComponent(NavigatorComponent);
}

// Called when the game starts or when spawned
void AUSCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AUSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("RotateRight", this, &AUSCharacter::OnCameraRotateRight);
    PlayerInputComponent->BindAxis("RotateUp", this, &AUSCharacter::OnCameraRotateUp);
    PlayerInputComponent->BindAxis("Zoom", this, &AUSCharacter::OnCameraZoom);
}

void AUSCharacter::OnCameraRotateRight(float Value)
{
    FQuat Rotation = FQuat::MakeFromEuler(FVector(0.f, 0.f, -Value * CameraRotationSpeed));
    SpringArmComponent->AddWorldRotation(Rotation);
}

void AUSCharacter::OnCameraRotateUp(float Value)
{
    FRotator CurrentRotation = SpringArmComponent->GetRelativeRotation();
    FRotator Rotation = FRotator::MakeFromEuler(FVector(0.f, -Value * CameraRotationSpeed, 0.f));
    FRotator NewRotation = CurrentRotation + Rotation;

    bool bInRange = UKismetMathLibrary::InRange_FloatFloat(NewRotation.Pitch, -60.f, -10.f);
    if (!bInRange)
    {
        return;
    }

    SpringArmComponent->AddLocalRotation(Rotation);
}

void AUSCharacter::OnCameraZoom(float Delta)
{
    float NewLength = SpringArmComponent->TargetArmLength - (Delta * CameraZoomSpeed);
    bool bInRange = UKismetMathLibrary::InRange_FloatFloat(NewLength, 100.f, 1200.f);
    if (!bInRange)
    {
        return;
    }

    SpringArmComponent->TargetArmLength = NewLength;
}
