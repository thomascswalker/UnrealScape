// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogAsset.h"

void UDialogAsset::Construct_Implementation()
{
    INFO(L"Default construction. Implement this!");
}

UDialogAsset* UDialogAsset::AddOneOption(const FString& Text, const EDialogType Type, int ThisStep,
                                         const FDialogOption& Option)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = Type;
    Dialog.Options.Add(Option);
    Conversation.Add(ThisStep, Dialog);
    return this;
}

UDialogAsset* UDialogAsset::AddTwoOptions(const FString& Text, int ThisStep, const FDialogOption& Option1,
                                          const FDialogOption& Option2)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = EDialogType::None;
    Dialog.Options.Add(Option1);
    Dialog.Options.Add(Option2);
    Conversation.Add(ThisStep, Dialog);
    return this;
}

UDialogAsset* UDialogAsset::AddThreeOptions(const FString& Text, int ThisStep, const FDialogOption& Option1,
                                            const FDialogOption& Option2, const FDialogOption& Option3)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = EDialogType::None;
    Dialog.Options.Add(Option1);
    Dialog.Options.Add(Option2);
    Dialog.Options.Add(Option3);
    Conversation.Add(ThisStep, Dialog);
    return this;
}

UDialogAsset* UDialogAsset::AddFourOptions(const FString& Text, int ThisStep, const FDialogOption& Option1,
                                           const FDialogOption& Option2, const FDialogOption& Option3,
                                           const FDialogOption& Option4)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = EDialogType::None;
    Dialog.Options.Add(Option1);
    Dialog.Options.Add(Option2);
    Dialog.Options.Add(Option3);
    Dialog.Options.Add(Option4);
    Conversation.Add(ThisStep, Dialog);
    return this;
}

UDialogAsset* UDialogAsset::AddEnd(int ThisStep)
{
    FDialog Dialog;
    Dialog.Type = EDialogType::End;
    Conversation.Add(ThisStep, Dialog);
    return this;
}

UDialogAsset* UDialogAsset::AddPlayer(const FString& Text, int ThisStep, int GoToStep)
{
    FDialogOption Option;
    Option.GoToStep = GoToStep;
    Option.Text = "Continue";

    AddOneOption(Text, EDialogType::Player, ThisStep, Option);

    return this;
}

UDialogAsset* UDialogAsset::AddNpc(const FString& Text, int ThisStep, int GoToStep)
{
    FDialogOption Option;
    Option.GoToStep = GoToStep;
    Option.Text = "Continue";

    AddOneOption(Text, EDialogType::Npc, ThisStep, Option);

    return this;
}