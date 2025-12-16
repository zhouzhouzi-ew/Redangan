#include "DialogueChoiceState.h"
#include "DialogueWidgetBase.h"

void UDialogueChoiceState::EnterState(UDialogueWidgetBase* Widget)
{
    Super::EnterState(Widget);

    if (Widget)
    {
        // 隐藏文本气泡，显示选项UI
        SetTextAndBubbleVisible(Widget, false);

        // 这里应该显示选项按钮
        // 具体实现取决于你的UI布局
        UE_LOG(LogTemp, Warning, TEXT("Choice state entered with %d options"), ChoiceTexts.Num());
    }
}

void UDialogueChoiceState::HandleBubbleClick(UDialogueWidgetBase* Widget)
{
    // 选择状态下不处理气泡点击
}

void UDialogueChoiceState::SetChoices(const TArray<FString>& InChoiceTexts, const TArray<FString>& InChoiceTargets)
{
    ChoiceTexts = InChoiceTexts;
    ChoiceTargets = InChoiceTargets;

    if (ChoiceTexts.Num() != ChoiceTargets.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Choice texts count %d != targets count %d"),
            ChoiceTexts.Num(), ChoiceTargets.Num());
    }
}



void UDialogueChoiceState::OnChoiceSelected(int32 ChoiceIndex, UDialogueWidgetBase* Widget)
{
    if (ChoiceIndex >= 0 && ChoiceIndex < ChoiceTargets.Num() && Widget)
    {
        SelectedChoiceIndex = ChoiceIndex;
        FString SelectedTarget = ChoiceTargets[ChoiceIndex];

        // 记录选择结果，可能会影响后续对话分支
        // 这里可以保存到Widget或Controller中
    }
}