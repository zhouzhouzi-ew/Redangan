#include "DialogueInvestigateState.h"
#include "DialogueWidgetBase.h"

void UDialogueInvestigateState::EnterState(UDialogueWidgetBase* Widget)
{
    Super::EnterState(Widget);

    if (Widget)
    {
        UE_LOG(LogTemp, Warning, TEXT("Entering investigate state for: %s"), *CurrentInvestigationTarget);

        // 加载调查对话
        LoadInvestigationDialogue(CurrentInvestigationTarget);

        if (InvestigationDialogues.Num() > 0)
        {
            // 显示第一句调查对话
            Widget->DialogueText->SetText(FText::FromString(InvestigationDialogues[0]->DialogueText));
            Widget->TextIndex = 0;
        }
    }
}

void UDialogueInvestigateState::HandleBubbleClick(UDialogueWidgetBase* Widget)
{
    if (!Widget)
        return;

    // 在调查状态下，点击气泡前进对话
    if (InvestigationDialogues.IsValidIndex(Widget->TextIndex + 1))
    {
        Widget->TextIndex++;
        Widget->DialogueText->SetText(FText::FromString(InvestigationDialogues[Widget->TextIndex]->DialogueText));
    }
    else
    {
        // 调查对话结束，返回主对话
        ReturnToMainDialogue(Widget);
    }
}

void UDialogueInvestigateState::HandleInvestigateClick(UDialogueWidgetBase* Widget, const FString& ButtonName)
{
    // 设置当前调查目标
    CurrentInvestigationTarget = ButtonName;

    // 通知父类切换状态
    if (Widget)
    {
        Widget->EnterInvestigateState(nullptr); // 假设已经在调查状态，可以重新进入
    }
}

bool UDialogueInvestigateState::CanTransitionTo(const FString& NewStateName) const
{
    // 调查状态可以转换回播放状态或隐藏状态
    return NewStateName == TEXT("Playing") || NewStateName == TEXT("Hidden");
}

void UDialogueInvestigateState::SetInvestigationData(UDataTable* NewInvestigationTable)
{
    InvestigationTable = NewInvestigationTable;

    if (InvestigationTable)
    {
        FString ContextString = TEXT("Investigation Context");
        TArray<FName> RowNames = InvestigationTable->GetRowNames();

        for (FName RowName : RowNames)
        {
            FDialogData* RowData = InvestigationTable->FindRow<FDialogData>(RowName, ContextString);
            if (RowData)
            {
                InvestigationDialogues.Add(RowData);
            }
        }
    }
}

void UDialogueInvestigateState::LoadInvestigationDialogue(const FString& TargetName)
{
    // 根据目标名称加载特定的调查对话
    // 这里需要根据你的项目结构实现
    UE_LOG(LogTemp, Warning, TEXT("Loading investigation dialogue for: %s"), *TargetName);
}

void UDialogueInvestigateState::ReturnToMainDialogue(UDialogueWidgetBase* Widget)
{
    if (Widget)
    {
        // 切换回播放状态
        Widget->EnterPlayingState();
    }
}