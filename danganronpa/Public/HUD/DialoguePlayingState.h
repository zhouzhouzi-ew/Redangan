
#pragma once

#include "BaseDialogueState.h"
#include "DialoguePlayingState.generated.h"

//Refactored with State Pattern 
UCLASS()
class DANGANRONPA_API UDialoguePlayingState : public UBaseDialogueState
{
    GENERATED_BODY()

public:
    virtual void EnterState(class UDialogueWidgetBase* Widget) override;
    virtual void HandleBubbleClick(class UDialogueWidgetBase* Widget) override;
    virtual FString GetStateName() const override { return TEXT("Playing"); }

private:
    void ProcessDialogueLine(class UDialogueWidgetBase* Widget);
};
