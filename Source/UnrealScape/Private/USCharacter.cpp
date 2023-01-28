// Fill out your copyright notice in the Description page of Project Settings.

#include "USCharacter.h"

// Sets default values
AUSCharacter::AUSCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->GravityScale = 0.f;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    SpringArmComponent->TargetArmLength = 300.f;
    SpringArmComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, -45.f, 0.f)));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    //GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
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
}
