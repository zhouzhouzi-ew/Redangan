#include "BaseDialogueState.h"
#include "DialogueWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TimerManager.h"

void UBaseDialogueState::EnterState(UDialogueWidgetBase* Widget)
{
    if (Widget)
    {
        bIsActive = true;
        UE_LOG(LogTemp, Warning, TEXT("Entering dialogue state: %s"), *GetStateName());
    }
}

void UBaseDialogueState::ExitState(UDialogueWidgetBase* Widget)
{
    bIsActive = false;
    UE_LOG(LogTemp, Warning, TEXT("Exiting dialogue state: %s"), *GetStateName());
}

void UBaseDialogueState::HandleBubbleClick(UDialogueWidgetBase* Widget)
{
    // 基类不执行任何操作，由子类重写
    UE_LOG(LogTemp, Warning, TEXT("Base state handling bubble click"));
}

void UBaseDialogueState::HandleInvestigateClick(UDialogueWidgetBase* Widget, const FString& ButtonName)
{
    // 基类不执行任何操作，由子类重写
    UE_LOG(LogTemp, Warning, TEXT("Base state handling investigate click: %s"), *ButtonName);
}

bool UBaseDialogueState::CanTransitionTo(const FString& NewStateName) const
{
    // 默认可以转换到任何状态
    return true;
}

void UBaseDialogueState::AdvanceDialogue(UDialogueWidgetBase* Widget)
{
    if (!Widget || !Widget->Dialogues.IsValidIndex(Widget->TextIndex))
        return;

    Widget->TextIndex++;

    if (Widget->TextIndex < Widget->Dialogues.Num())
    {
        Widget->DialogueText->SetText(FText::FromString(Widget->Dialogues[Widget->TextIndex]->DialogueText));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue ended"));
    }
}

void UBaseDialogueState::SetTextAndBubbleVisible(UDialogueWidgetBase* Widget, bool bVisible)
{
    if (!Widget || !Widget->TextBubble || !Widget->DialogueText)
        return;

    ESlateVisibility Visibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
    Widget->TextBubble->SetVisibility(Visibility);
    Widget->DialogueText->SetVisibility(Visibility);
}

void UBaseDialogueState::ShowPortrait(UDialogueWidgetBase* Widget, const FString& CharacterName)
{
    if (!Widget) return;

    UImage* Portrait = Cast<UImage>(Widget->GetWidgetFromName(FName(CharacterName)));
    if (Portrait)
    {
        Portrait->SetVisibility(ESlateVisibility::Visible);
        Widget->CurrentPortrait = Portrait;
    }
}

void UBaseDialogueState::HidePortrait(UDialogueWidgetBase* Widget)
{
    if (!Widget || !Widget->CurrentPortrait) return;

    Widget->CurrentPortrait->SetVisibility(ESlateVisibility::Hidden);
    Widget->CurrentPortrait = nullptr;
}