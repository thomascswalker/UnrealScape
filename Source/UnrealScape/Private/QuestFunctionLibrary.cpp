// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestFunctionLibrary.h"

UQuestComponent* UQuestFunctionLibrary::GetQuestComponent(const UObject* Context)
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
    return Controller->QuestComponent;
}

UQuest* UQuestFunctionLibrary::GetQuest(const UObject* Context, EQuestList Id)
{
    UQuestComponent* Component = GetQuestComponent(Context);
    if (!Component)
    {
        return nullptr;
    }
    return Component->GetQuest(Id);
}

bool UQuestFunctionLibrary::IsQuestStarted(const UObject* Context, EQuestList Id)
{
    UQuest* Quest = GetQuest(Context, Id);
    if (!Quest)
    {
        return false;
    }
    return Quest->CurrentStep != 0;
}

bool UQuestFunctionLibrary::IsQuestComplete(const UObject* Context, EQuestList Id)
{
    UQuest* Quest = GetQuest(Context, Id);
    if (!Quest)
    {
        return false;
    }
    return Quest->bComplete;
}

void UQuestFunctionLibrary::StartQuest(const UObject* Context, EQuestList Id)
{
    UQuest* Quest = GetQuest(Context, Id);
    if (!Quest)
    {
        return;
    }
    Quest->Start();
}

void UQuestFunctionLibrary::CompleteQuest(const UObject* Context, EQuestList Id)
{
    UQuest* Quest = GetQuest(Context, Id);
    if (!Quest)
    {
        return;
    }
    Quest->Complete();
}

int UQuestFunctionLibrary::GetQuestStep(const UObject* Context, EQuestList Id)
{
    UQuest* Quest = GetQuest(Context, Id);
    if (!Quest)
    {
        return -1;
    }
    return Quest->CurrentStep;
}

void UQuestFunctionLibrary::SetQuestStep(const UObject* Context, EQuestList Id, int Step)
{
    UQuest* Quest = GetQuest(Context, Id);
    if (!Quest)
    {
        return;
    }
    Quest->CurrentStep = Step;
    Quest->Update(Quest->CurrentStep);
}