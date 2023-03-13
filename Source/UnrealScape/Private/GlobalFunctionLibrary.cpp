// Fill out your copyright notice in the Description page of Project Settings.

#include "GlobalFunctionLibrary.h"

UChatbox* UGlobalFunctionLibrary::GetChatbox(UObject* Context)
{
    TArray<UUserWidget*> FoundWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Context, FoundWidgets, UChatbox::StaticClass(), false);
    if (FoundWidgets.Num() == 0)
    {
        return nullptr;
    }
    return Cast<UChatbox>(FoundWidgets[0]);
}

void UGlobalFunctionLibrary::AddChatboxMessage(UObject* Context, const FText Text)
{

    UChatbox* Chatbox = GetChatbox(Context);
    if (!Chatbox)
    {
        WARNING(L"Chatbox not found.");
        return;
    }
    Chatbox->AddItem(Text);
}

FVector UGlobalFunctionLibrary::GetPlayerLocation(UObject* Context)
{
    UWorld* World = Context->GetWorld();
    APlayerController* Controller = UGameplayStatics::GetPlayerController(World, 0);
    if (!Controller)
    {
        return FVector::Zero();
    }
    FVector Start = Controller->GetPawn()->GetActorLocation();
    FVector End = (FVector::DownVector * 10000.f) + Start;

    TArray<AActor*> ActorsToIgnore;
    FCollisionQueryParams TraceParams;
    FHitResult HitResult;
    const bool bBlockingHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Terrain, TraceParams);
    if (bBlockingHit)
    {
        return HitResult.Location;
    }
    else
    {
        return FVector::Zero();
    }
}