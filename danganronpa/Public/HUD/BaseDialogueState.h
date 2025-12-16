
#pragma once

#include "CoreMinimal.h"
#include "DialogueState.h"
#include "BaseDialogueState.generated.h"

UCLASS(Abstract)
class DANGANRONPA_API UBaseDialogueState : public UObject, public IDialogueState
{
    GENERATED_BODY()

public:
    // IDialogueState接口实现
    virtual void EnterState(class UDialogueWidgetBase* Widget) override;
    virtual void ExitState(class UDialogueWidgetBase* Widget) override;
    virtual void HandleBubbleClick(class UDialogueWidgetBase* Widget) override;
    virtual void HandleInvestigateClick(class UDialogueWidgetBase* Widget, const FString& ButtonName) override;
    virtual FString GetStateName() const override { return TEXT("BaseState"); }
    virtual bool CanTransitionTo(const FString& NewStateName) const override { return true; }

protected:
    // 通用辅助方法
    void AdvanceDialogue(class UDialogueWidgetBase* Widget);
    void ShowPortrait(class UDialogueWidgetBase* Widget, const FString& CharacterName);
    void HidePortrait(class UDialogueWidgetBase* Widget);
    void SetTextAndBubbleVisible(class UDialogueWidgetBase* Widget, bool bVisible);

    // 状态数据
    int32 CurrentDialogueIndex = 0;
    TArray<FDialogData*> CurrentDialogues;
};