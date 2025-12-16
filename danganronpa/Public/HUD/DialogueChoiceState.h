#pragma once

#include "BaseDialogueState.h"
#include "DialogueChoiceState.generated.h"

UCLASS()
class DANGANRONPA_API UDialogueChoiceState : public UBaseDialogueState
{
    GENERATED_BODY()

public:
    virtual void EnterState(class UDialogueWidgetBase* Widget) override;
    virtual void HandleBubbleClick(class UDialogueWidgetBase* Widget) override;
    virtual FString GetStateName() const override { return TEXT("Choice"); }

    // 设置选项数据
    void SetChoices(const TArray<FString>& ChoiceTexts, const TArray<FString>& ChoiceTargets);

private:
    TArray<FString> ChoiceTexts;
    TArray<FString> ChoiceTargets; // 每个选项对应的对话目标或状态
    int32 SelectedChoiceIndex = -1;

    void ShowChoiceUI(class UDialogueWidgetBase* Widget);
    void OnChoiceSelected(int32 ChoiceIndex, class UDialogueWidgetBase* Widget);
};