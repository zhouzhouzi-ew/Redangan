
#pragma once

#include "BaseDialogueState.h"
#include "DialogueInvestigateState.generated.h"

UCLASS()
class DANGANRONPA_API UDialogueInvestigateState : public UBaseDialogueState
{
    GENERATED_BODY()

public:
    virtual void EnterState(class UDialogueWidgetBase* Widget) override;
    virtual void HandleInvestigateClick(class UDialogueWidgetBase* Widget, const FString& ButtonName) override;
    virtual FString GetStateName() const override { return TEXT("Investigate"); }
    virtual bool CanTransitionTo(const FString& NewStateName) const override;

    // 设置调查数据表
    void SetInvestigationData(UDataTable* NewInvestigationTable);

private:
    UPROPERTY()
    TObjectPtr<UDataTable> InvestigationTable;

    TArray<FDialogData*> InvestigationDialogues;
    FString CurrentInvestigationTarget;

    void LoadInvestigationDialogue(const FString& TargetName);
    void ReturnToMainDialogue(class UDialogueWidgetBase* Widget);
};
