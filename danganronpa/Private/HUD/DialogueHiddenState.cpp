#include "DialogueHiddenState.h"
#include "DialogueWidgetBase.h"
#include "TimerManager.h"

//Refactored with State Pattern 
void UDialogueHiddenState::EnterState(UDialogueWidgetBase* Widget)
{
    Super::EnterState(Widget);

    if (Widget)
    {
        // 隐藏UI元素
        SetTextAndBubbleVisible(Widget, false);

        // 设置自动返回定时器
        if (AutoReturnDelay > 0.0f)
        {
            Widget->GetWorld()->GetTimerManager().SetTimer(
                AutoReturnTimer,
                [this, Widget]() { OnAutoReturn(Widget); },
                AutoReturnDelay,
                false
            );
        }
    }
}

void UDialogueHiddenState::HandleBubbleClick(UDialogueWidgetBase* Widget)
{
    // 清除定时器
    if (Widget)
    {
        Widget->GetWorld()->GetTimerManager().ClearTimer(AutoReturnTimer);
    }

    // 可以选择手动触发返回
}

void UDialogueHiddenState::SetAutoReturnDelay(float DelaySeconds)
{
    AutoReturnDelay = DelaySeconds;
}

void UDialogueHiddenState::OnAutoReturn(UDialogueWidgetBase* Widget)
{
    if (Widget)
    {
        // 触发返回播放状态或下一个状态
        Widget->EnterPlayingState();
    }
}
