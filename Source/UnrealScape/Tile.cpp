// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

ATile::ATile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneComponent;

    // Construct the static mesh component
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    auto Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (Mesh.Object)
    {
        StaticMeshComponent->SetStaticMesh(Mesh.Object);
        StaticMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, .1));
    }
    StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    // Text render
    TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TileText"));
    TextRenderComponent->SetHorizontalAlignment(EHTA_Center);
    TextRenderComponent->SetWorldSize(10.f);
    TextRenderComponent->AddLocalTransform(FTransform(FVector(0.f, 0.f, 10.f)));
    TextRenderComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, 90.f, 90.f)));
    TextRenderComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool ATile::IsWalkable()
{
    FVector Start = GetActorLocation();
    FVector End = Start + FVector(0, 0, 25);

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());
    FHitResult HitResult;
    const bool BlockingHit = UKismetSystemLibrary::SphereTraceSingle(
        this, Start, End, 25.f, UEngineTypes::ConvertToTraceType(ECC_WorldStatic), false, ActorsToIgnore,
        EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 60.f);

    return !BlockingHit;
}

void ATile::SetText(FText& Text)
{
    TextRenderComponent->SetText(Text);
}
