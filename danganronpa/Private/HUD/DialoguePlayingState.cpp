#include "DialoguePlayingState.h"
#include "DialogueWidgetBase.h"

//Refactored with State Pattern 
void UDialoguePlayingState::EnterState(UDialogueWidgetBase* Widget)
{
    Super::EnterState(Widget);

    if (Widget)
    {
        // 确保文本和气泡可见
        SetTextAndBubbleVisible(Widget, true);

        // 如果当前对话行需要显示立绘
        if (Widget->TextIndex < Widget->Dialogues.Num() && Widget->Dialogues[Widget->TextIndex]->intro)
        {
            Widget->SetPortraitVisible();
        }
    }
}

void UDialoguePlayingState::HandleBubbleClick(UDialogueWidgetBase* Widget)
{
    if (!Widget || !Widget->Dialogues.IsValidIndex(Widget->TextIndex))
        return;

    FDialogData* CurrentDialogue = Widget->Dialogues[Widget->TextIndex];

    if (CurrentDialogue->out)
    {
        // 如果是out标记，进入隐藏状态
        UE_LOG(LogTemp, Warning, TEXT("Sensed Out - transitioning to hidden state"));
        Widget->SetPortraitHidden();
        SetTextAndBubbleVisible(Widget, false);

        // 延迟后返回播放状态
        Widget->GetWorld()->GetTimerManager().SetTimer(
            Widget->TimerHandle1,
            [Widget]()
            {
                if (Widget->TextIndex + 1 < Widget->Dialogues.Num())
                {
                    Widget->TextIndex++;
                    Widget->SetTextAndBubbleVisible();
                }
            },
            4.0f, // 延迟4秒
            false
        );
    }
    else if (Widget->TextIndex + 1 < Widget->Dialogues.Num() &&
        Widget->Dialogues[Widget->TextIndex + 1]->intro)
    {
        // 下一行需要显示立绘
        Widget->SetPortraitHidden();
        Widget->TextIndex++;
        Widget->SetPortraitVisible();
        Widget->DialogueText->SetText(FText::FromString(Widget->Dialogues[Widget->TextIndex]->DialogueText));
    }
    else
    {
        // 普通对话前进
        AdvanceDialogue(Widget);
    }
}

void UDialoguePlayingState::ProcessDialogueLine(UDialogueWidgetBase* Widget)
{
    // 处理特殊对话行逻辑
    if (Widget && Widget->Dialogues.IsValidIndex(Widget->TextIndex))
    {
        FDialogData* Dialogue = Widget->Dialogues[Widget->TextIndex];

        // 可以在这里添加特殊处理，比如播放声音、触发事件等
        if (!Dialogue->CharacterName.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("Character: %s"), *Dialogue->CharacterName);
        }
    }
}
