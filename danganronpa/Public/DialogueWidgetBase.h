#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include"HUD/DTReader.h"
#include "DialogueWidgetBase.generated.h"
class UButton;
class UTextBlock;
class UWidgetAnimation;
class UImage;
/**
 *
 */
UCLASS()
class DANGANRONPA_API UDialogueWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton>TextBubble;

	// 绑定的TextBlock
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueText;

	int32 TextIndex;

	TArray<FDialogData*>Dialogues;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> MainDialogue;//                            DataTables

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> KototamaTable;

	virtual void InitializePortraits();

	TArray<FName>_rowNames;

	UFUNCTION()
	virtual void OnBubbleClicked();



	//让文本框重新出现且内容往前，delay为出现的延迟时间
	void TextMoveOn(float delay = 4.0f);

	TObjectPtr<UImage> CurrentPortrait;

	void SetPortraitVisible();
	void SetPortraitHidden();

	//获取定时器管理器
	FTimerHandle TimerHandle1;
	FTimerHandle TimerHandle2;

	void SetTextAndBubbleHidden();

	void SetTextAndBubbleVisible();

	bool EndMap = false;
	// ============ 状态管理 ============
	UFUNCTION(BlueprintCallable, Category = "Dialogue State")
	void ChangeDialogueState(TSubclassOf<UBaseDialogueState> NewStateClass);

	UFUNCTION(BlueprintCallable, Category = "Dialogue State")
	FString GetCurrentStateName() const;

	// 状态快捷方法
	void EnterPlayingState();
	void EnterInvestigateState(UDataTable* InvestigationTable = nullptr);
	void EnterChoiceState(const TArray<FString>& Choices, const TArray<FString>& Targets);
	void EnterHiddenState(float AutoReturnDelay = 4.0f);

	// 获取当前状态（供子类访问）
	UFUNCTION(BlueprintCallable, Category = "Dialogue State")
	UBaseDialogueState* GetCurrentState() const { return CurrentDialogueState; }
protected:
	// 状态管理
	UPROPERTY()
	UBaseDialogueState* CurrentDialogueState;

	// 状态实例缓存
	UPROPERTY()
	TMap<FString, UBaseDialogueState*> DialogueStateInstances;

	// 创建状态实例
	UBaseDialogueState* CreateStateInstance(TSubclassOf<UBaseDialogueState> StateClass);

	// 状态转换事件（可被子类重写）
	virtual void OnStateChanged(const FString& PreviousState, const FString& NewState);

};
