
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DTReader.h"
#include "DialogueState.generated.h"

//Refactored with State Pattern 
UINTERFACE(MinimalAPI)
class UDialogueState : public UInterface
{
    GENERATED_BODY()
};

class DANGANRONPA_API IDialogueState
{
    GENERATED_BODY()

public:
    // 进入状态
    virtual void EnterState(class UDialogueWidgetBase* Widget) = 0;

    // 退出状态
    virtual void ExitState(class UDialogueWidgetBase* Widget) = 0;

    // 处理气泡点击
    virtual void HandleBubbleClick(class UDialogueWidgetBase* Widget) = 0;

    // 处理调查按钮点击
    virtual void HandleInvestigateClick(class UDialogueWidgetBase* Widget, const FString& ButtonName) = 0;

    // 获取状态名称
    virtual FString GetStateName() const = 0;

    // 是否可以切换到其他状态
    virtual bool CanTransitionTo(const FString& NewStateName) const = 0;
};
