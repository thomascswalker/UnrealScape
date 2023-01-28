// Fill out your copyright notice in the Description page of Project Settings.

#include "USCharacter.h"

// Sets default values
AUSCharacter::AUSCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Modify default character movement values
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Spring Arm and Camera
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    SpringArmComponent->TargetArmLength = 1200.f;
    SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, -45.f, 0.f)));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    // Player mesh
    UStaticMeshComponent* PlayerSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    auto SphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (SphereMesh.Object)
    {
        PlayerSphere->SetStaticMesh(SphereMesh.Object);
    }
    PlayerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PlayerSphere->SetupAttachment(RootComponent);

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
}
