// Fill out your copyright notice in the Description page of Project Settings.

#include "HintFunctionLibrary.h"

UHintComponent* UHintFunctionLibrary::GetHintComponent(const UObject* Context)
{
    if (!IsValid(Context) || !Context->ImplementsGetWorld())
    {
        FString Message =
            FString::Printf(L"Object %s is not valid or does not implement GetWorld().", *Context->GetName());
        FATAL(Message);
        return nullptr;
    }
    AUSPlayerController* Controller =
        Cast<AUSPlayerController>(UGameplayStatics::GetPlayerController(Context->GetWorld(), 0));
    if (!Controller)
    {
        return nullptr;
    }
    return Controller->HintComponent;
}

void UHintFunctionLibrary::CreateHint(const UObject* Context, FVector Location, bool bClearHints)
{
    UHintComponent* Component = GetHintComponent(Context);
    if (!Component)
    {
        return;
    }
    Component->CreateHint(Location, bClearHints);
}

void UHintFunctionLibrary::RemoveHint(const UObject* Context, int Index)
{
    UHintComponent* Component = GetHintComponent(Context);
    if (!Component)
    {
        return;
    }
    Component->RemoveHint(Index);
}

void UHintFunctionLibrary::ClearHints(const UObject* Context)
{
    UHintComponent* Component = GetHintComponent(Context);
    if (!Component)
    {
        return;
    }
    Component->ClearHints();
}