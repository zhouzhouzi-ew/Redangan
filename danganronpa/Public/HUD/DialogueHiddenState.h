
#pragma once

#include "BaseDialogueState.h"
#include "DialogueHiddenState.generated.h"

UCLASS()
class DANGANRONPA_API UDialogueHiddenState : public UBaseDialogueState
{
    GENERATED_BODY()

public:
    virtual void EnterState(class UDialogueWidgetBase* Widget) override;
    virtual void HandleBubbleClick(class UDialogueWidgetBase* Widget) override;
    virtual FString GetStateName() const override { return TEXT("Hidden"); }

    // 设置延迟后自动返回播放状态
    void SetAutoReturnDelay(float DelaySeconds);

private:
    FTimerHandle AutoReturnTimer;
    float AutoReturnDelay = 4.0f;

    void OnAutoReturn(class UDialogueWidgetBase* Widget);
};
